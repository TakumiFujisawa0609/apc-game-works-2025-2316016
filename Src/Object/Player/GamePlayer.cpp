#include "../../Utility/Utility.h"
#include "../../Manager/Camera.h"
#include "../../Manager/KeyConfig.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/SoundManager.h"
#include "../../Manager/DataBank.h"
#include "../Common/Gravity.h"
#include "../Common/Geometry/Sphere.h"
#include "../Common/AnimationController.h"
#include "../Light/PointLight.h"
#include "../Enemy/EnemyBase.h"
#include "../Enemy/Attack/SubObject/SubObjectBase.h"
#include "PlayerShot.h"
#include "GamePlayer.h"

GamePlayer::GamePlayer(int playerNum) : PlayerBase(playerNum)//, keyIns_(KeyConfig::GetInstance())
{
	hp_ = MAX_HP;
	avoidCoolTime_ = 0.0f;
	attackDeley_ = 0.0f;
	damageInvincibleTime_ = 0.0f;
	damageTime_ = 0.0f;
	rimPow_ = RIM_MIN_POW;
	sumDamage_ = 0.0f;
	damageNum_ = 0;
	avoidNum_ = 0;
	avoidSaccessNum_ = 0;
	jumpNum_ = 0;
	jumpSaccessNum_ = 0;

	gravity_ = std::make_unique<Gravity>();
	gravity_->ChengeState(Gravity::STATE::NONE);
	gravity_->SetDir(Utility::DIR_D);
	gravity_->ChengeState(Gravity::STATE::JUMP);
	controlType_ = DataBank::GetInstance().GetControlType();
	isDeath_ = false;
	healDeray_ = 0.0f;
	headPos_ = Utility::VECTOR_ZERO;
	landPos_ = Utility::VECTOR_ZERO;

	std::unique_ptr<Geometry>geo = std::make_unique<Sphere>(transform_->pos, RADIUS);
	MakeCollider(Collider::TAG::PLAYER, std::move(geo), { Collider::TAG::PLAYER,Collider::TAG::PLAYER_ATTACK ,Collider::TAG::PLAYER_LAND });
	geo = std::make_unique<Sphere>(headPos_, RADIUS);
	MakeCollider(Collider::TAG::PLAYER, std::move(geo), { Collider::TAG::PLAYER,Collider::TAG::PLAYER_ATTACK ,Collider::TAG::PLAYER_LAND });
	geo = std::make_unique<Sphere>(landPos_, RADIUS);
	MakeCollider(Collider::TAG::PLAYER_LAND, std::move(geo), { Collider::TAG::PLAYER,Collider::TAG::PLAYER_ATTACK ,Collider::TAG::PLAYER_LAND });
	isAvoidSaccess_ = false;
	SetupStateChange();
	avoidSaccessTime_ = -1.0f;
	footNum_ = 0;
	ChangeState(STATE::IDLE, true);
}

GamePlayer::~GamePlayer(void)
{
}

void GamePlayer::Init(void)
{
}

void GamePlayer::Update(void)
{
	if (playerNum_ != 0)
	{
		return;
	}
	if (state_ != STATE::DAMAGE)
	{
		if (hp_ < 0.0f)
		{
			isDeath_ = true;
		}
		else
		{
			isDeath_ = false;
		}
	}
	prePos_ = transform_->pos;
	//デルタタイムを取得し各種時間関係を更新する
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();
	avoidCoolTime_ -= deltaTime;
	attackDeley_ -= deltaTime;
	damageTime_ -= deltaTime;
	damageInvincibleTime_ -= deltaTime;
	healDeray_ -= deltaTime;
	//状態ごとの更新
	stateUpdate_();
	//重力の更新
	gravity_->Update();
	//重力の適用
	AplayGravity();
	//移動制限
	MoveLimit();
	Rotation();
	Heal();
	transform_->Update();
	headPos_ = MV1GetFramePosition(transform_->modelId, HEAD_BONE_NO);
	landPos_ = transform_->pos;
	landPos_.y = MOVE_LIMIT_MIN.y;
	//攻撃のクールタイム中ではなく攻撃ボタンを押したら攻撃する
	if (keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_ATTACK, KeyConfig::JOYPAD_NO::PAD1, controlType_) && attackDeley_ < 0.0f)
	{
		if (state_ == STATE::IDLE || state_ == STATE::MOVE)
		{
			ChangeState(STATE::ATTACK);
		}
	}
	//球の更新
	for (auto& shot : shots_)
	{
		shot->Update();
		if (shot->IsDead())shot = nullptr;
	}
	Utility::EraseVectorAllay(shots_);
	animCtrl_->Update();
	auto cameraPos = SceneManager::GetInstance().GetCamera().GetPos();
	material_->SetConstBufPS(1, { cameraPos.x,cameraPos.y,cameraPos.z,rimPow_ });
}

void GamePlayer::Draw(void)
{
	PlayerBase::Draw();
	for (auto& shot : shots_)
	{
		shot->Draw();
	}
}

void GamePlayer::UIDraw(void)
{
}

void GamePlayer::OnHit(const std::weak_ptr<Collider> _hitCol, VECTOR hitPos)
{

	std::shared_ptr<Collider> hitCol = _hitCol.lock();
	if (!IsDamageHit())
	{
		if (state_ == STATE::AVOID && avoidSaccessTime_ < 0.0f)
		{
			SaccessAvoid();
		}
		return;
	}
	if (hitCol->GetGeometry().GetType() == Geometry::GEOMETRY_TYPE::CIRCUMFERENCE && state_ == STATE::JUMP)
	{
		SaccessJumpAvoid();
		return;
	}
	Collider::TAG tag = hitCol->GetTag();
	auto& hit = hitCol->GetParent();
	float damage = 0.0f;
	VECTOR dir = Utility::VECTOR_ZERO;
	for (auto& colParam : colParam_)
	{
		auto& geo = colParam.geometry_;
		auto& collider = colParam.collider_;
		if (!collider->IsHit())
		{
			continue;
		}
		if (collider->GetTag() == Collider::TAG::PLAYER_LAND)
		{
			if (tag != Collider::TAG::ENEMY_ATTACK)
			{
				return;
			}
			auto& attack = dynamic_cast<SubObjectBase&>(hit);
			return;

		}
		auto& sph = dynamic_cast<Sphere&>(*geo);
		VECTOR pos = sph.GetPos();
		dir = VSub(pos, hitPos);
	}
	switch (tag)
	{
	case Collider::TAG::PLAYER:
	case Collider::TAG::PLAYER_ATTACK:
	case Collider::TAG::PLAYER_LAND:
	case Collider::TAG::GATE:
		return;
		break;
	case Collider::TAG::ENEMY:
		damage = ENEMY_HIT_DAMAGE;
		break;
	case Collider::TAG::ENEMY_ATTACK:
	{
		auto& subObject = dynamic_cast<SubObjectBase&>(hit);
		damage = subObject.GetDamage();
	}
	break;
	default:
		break;
	}
	SoundManager::GetInstance().Play(SoundManager::SRC::DAMAGE, Sound::TIMES::ONCE);
	Damage(damage, dir);
}

bool GamePlayer::ChangeState(STATE state, bool isAbsolute)
{
	if (state_ != state || isAbsolute == true)
	{
		state_ = state;
		stateChanges_[state_]();
		return true;
	}
	return false;
}

void GamePlayer::Damage(float damage, VECTOR dir)
{
	if (ChangeState(STATE::DAMAGE))
	{
		material_->SetConstBufPS(0, Utility::COLOR_F2FLOAT4(DAMAGE_COLOR_TIMES));
		hp_ -= damage;
		sumDamage_ += damage;
		damageNum_++;
		healDeray_ = DAMAGE_HEAL_DERAY;
		damageDir_ = dir;
		damageDir_.y = 0.0f;
		damageDir_ = VNorm(damageDir_);
		if (gravity_->GetState() == Gravity::STATE::NONE)
		{
			gravity_->SetInitPower(DAMAGE_POW);
			gravity_->ChengeState(Gravity::STATE::JUMP);
		}
	}
}

bool GamePlayer::IsDamageHit(void)
{
	//回避状態じゃなく無敵状態じゃなかったらダメージを食らう
	return state_ != STATE::AVOID && damageInvincibleTime_ < 0.0f;
}

std::vector<VECTOR> GamePlayer::GetCollisionSpherePositions(void)
{
	std::vector<VECTOR> positions;
	positions.push_back(transform_->pos);
	positions.push_back(MV1GetFramePosition(transform_->modelId, HEAD_BONE_NO));
	return positions;
}

void GamePlayer::PlayerMove(void)
{
	SceneManager& sceneIns = SceneManager::GetInstance();
	Camera& cam = sceneIns.GetCamera();
	//カメラの前方向を取得
	VECTOR front = VSub(cam.GetTargetPos(), cam.GetPos());
	front.y = 0.0f;
	front = VNorm(front);
	//カメラの左方向を取得
	VECTOR left = front;
	std::swap(left.x, left.z);
	left.x = -left.x;

	VECTOR dir = Utility::VECTOR_ZERO;
	//キーボードでの移動処理
	if (keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_UP, KeyConfig::JOYPAD_NO::PAD1, controlType_))
	{
		dir = VAdd(dir, front);
	}
	if (keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_DOWN, KeyConfig::JOYPAD_NO::PAD1, controlType_))
	{
		dir = VAdd(dir, VScale(front, -1));
	}
	if (keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_RIGHT, KeyConfig::JOYPAD_NO::PAD1, controlType_))
	{
		dir = VAdd(dir, VScale(left, -1));
	}
	if (keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_LEFT, KeyConfig::JOYPAD_NO::PAD1, controlType_))
	{
		dir = VAdd(dir, left);
	}
	if (dir.x != 0.0f || dir.z != 0.0f)
	{
		dir = VNorm(dir);
		transform_->pos = VAdd(transform_->pos, VScale(dir, MOVE_SPEED));
	}

	if (controlType_ == KeyConfig::TYPE::KEYBORD_MOUSE)
	{
		return;
	}
	//PADのスティック情報を取得
	auto stick2D = (keyIns_.GetKnockLStickSize(KeyConfig::JOYPAD_NO::PAD1));
	if (stick2D.x == 0.0f && stick2D.y == 0.0f)
	{
		return;
	}
	//スティック情報を3D情報に変更
	auto stick3D = Utility::Normalize(stick2D);
	//座標を更新する
	transform_->pos = VAdd(transform_->pos, VScale(front, stick3D.y * MOVE_SPEED * -1));
	transform_->pos = VAdd(transform_->pos, VScale(left, stick3D.x * MOVE_SPEED * -1));
}

void GamePlayer::MoveLimit(void)
{
	auto& pos = transform_->pos;
	VECTOR prePos = pos;
	pos.y = std::max(std::min(MOVE_LIMIT_MAX.y, pos.y), MOVE_LIMIT_MIN.y);
	if (prePos.y < pos.y && state_ != STATE::DAMAGE)
	{
		gravity_->ChengeState(Gravity::STATE::NONE);
		SoundManager& ins = SoundManager::GetInstance();
		ins.Play(SoundManager::SRC::LAND, Sound::TIMES::ONCE);
	}
}

void GamePlayer::AplayGravity(void)
{
	transform_->pos = VAdd(transform_->pos, VScale(gravity_->GetDir(), gravity_->GetPower()));
}

void GamePlayer::SetupStateChange(void)
{
	stateChanges_[(STATE::IDLE)] = std::bind(&GamePlayer::StateChangeIdle, this);
	stateChanges_[(STATE::MOVE)] = std::bind(&GamePlayer::StateChangeMove, this);
	stateChanges_[(STATE::JUMP)] = std::bind(&GamePlayer::StateChangeJump, this);
	stateChanges_[(STATE::AVOID)] = std::bind(&GamePlayer::StateChangeAvoid, this);
	//stateChanges_[(STATE::CHARGE)] = std::bind(&GamePlayer::StateChangeCharge, this);
	stateChanges_[(STATE::ATTACK)] = std::bind(&GamePlayer::StateChangeAttack, this);
	stateChanges_[(STATE::DAMAGE)] = std::bind(&GamePlayer::StateChangeDamage, this);
	stateChanges_[(STATE::DEAD)] = std::bind(&GamePlayer::StateChangeDead, this);
}

bool GamePlayer::IsPushMoveKey(void)
{
	if (keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_DOWN, KeyConfig::JOYPAD_NO::PAD1, controlType_)
		|| keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_UP, KeyConfig::JOYPAD_NO::PAD1, controlType_)
		|| keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_LEFT, KeyConfig::JOYPAD_NO::PAD1, controlType_)
		|| keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_RIGHT, KeyConfig::JOYPAD_NO::PAD1, controlType_))
	{
		return true;
	}
	if (controlType_ == KeyConfig::TYPE::KEYBORD_MOUSE)
	{
		return false;
	}
	auto stick = keyIns_.GetKnockLStickSize(KeyConfig::JOYPAD_NO::PAD1);
	if (stick.x != 0.0f || stick.y != 0.0f)
	{
		return true;
	}
	return false;
}

void GamePlayer::CreateShot(void)
{
	std::unique_ptr<PlayerShot> shot = std::make_unique<PlayerShot>(transform_->pos, SceneManager::GetInstance().GetCamera().GetTargetPos());
	shots_.push_back(std::move(shot));
	SoundManager::GetInstance().Play(SoundManager::SRC::PSHOT_THROW, Sound::TIMES::ONCE);
}

void GamePlayer::Rotation(void)
{
	VECTOR dir = Utility::VECTOR_ZERO;
	switch (state_)
	{
	case PlayerBase::STATE::IDLE:
	case PlayerBase::STATE::DAMAGE:
	case PlayerBase::STATE::DEAD:
		return;
		break;
	case PlayerBase::STATE::MOVE:
	case PlayerBase::STATE::JUMP:
	case PlayerBase::STATE::AVOID:
		dir = VSub(transform_->pos, prePos_);
		break;
	case PlayerBase::STATE::ATTACK:
		dir = VSub(SceneManager::GetInstance().GetCamera().GetTargetPos(), transform_->pos);
		break;
	default:
		break;
	}
	if (dir.x == 0.0f && dir.z == 0.0f)
	{
		return;
	}
	dir = VNorm(dir);
	float angleY = atan2f(-dir.x, -dir.z);
	transform_->rot.y = angleY;
	transform_->quaRot = Quaternion(VGet(0.0f, angleY, 0.0f));
}

void GamePlayer::Heal(void)
{
	if (healDeray_ < 0.0f)
	{
		hp_ += HEAL_PER_SEC * SceneManager::GetInstance().GetDeltaTime();
	}
	if (hp_ > MAX_HP)
	{
		hp_ = MAX_HP;
	}
}

void GamePlayer::PlayFootSound(void)
{
	SoundManager& ins = SoundManager::GetInstance();
	if (footNum_ % 2 == 0)
	{
		ins.Play(SoundManager::SRC::FOOT_STEP1, Sound::TIMES::ONCE);
	}
	else
	{
		ins.Play(SoundManager::SRC::FOOT_STEP2, Sound::TIMES::ONCE);
	}
	footNum_++;
}

void GamePlayer::StopFootSound(void)
{
	SoundManager& ins = SoundManager::GetInstance();
	ins.Stop(SoundManager::SRC::FOOT_STEP1);
	ins.Stop(SoundManager::SRC::FOOT_STEP2);
}

bool GamePlayer::IsPlayFootSound(void)
{
	SoundManager& ins = SoundManager::GetInstance();
	return ins.CheckMove(SoundManager::SRC::FOOT_STEP1) || ins.CheckMove(SoundManager::SRC::FOOT_STEP2);
}

void GamePlayer::SaccessAvoid(void)
{
	isAvoidSaccess_ = true;
	avoidSaccessTime_ = avoidTime_;
	rimPow_ = RIM_MAX_POW;
	avoidSaccessNum_++;
}

void GamePlayer::SaccessJumpAvoid(void)
{
	//ジャンプ回避成功時の処理
	jumpSaccessNum_++;
}

void GamePlayer::StateChangeIdle(void)
{
	stateUpdate_ = std::bind(&GamePlayer::StateUpdateIdle, this);
	animCtrl_->Play((int)STATE::IDLE, 0.0f, -1.0f, 0.1f);
}

void GamePlayer::StateChangeMove(void)
{
	stateUpdate_ = std::bind(&GamePlayer::StateUpdateMove, this);
	PlayFootSound();
	animCtrl_->Play((int)STATE::MOVE);
}

void GamePlayer::StateChangeJump(void)
{
	SoundManager& ins = SoundManager::GetInstance();
	ins.Play(SoundManager::SRC::JAMP, Sound::TIMES::ONCE);
	gravity_->ChengeState(Gravity::STATE::JUMP);
	gravity_->SetInitPower(JUMP_POW);
	jumpNum_++;
	stateUpdate_ = std::bind(&GamePlayer::StateUpdateJump, this);
	animCtrl_->Play((int)STATE::JUMP, false, 30.0f, 90.0f, 0.1f, false, true);

};

void GamePlayer::StateChangeAvoid(void)
{
	stateUpdate_ = std::bind(&GamePlayer::StateUpdateAvoid, this);
	avoidTime_ = AVOID_TIME;
	avoidNum_++;
	SceneManager& sceneIns = SceneManager::GetInstance();
	Camera& cam = sceneIns.GetCamera();
	if (IsPushMoveKey())
	{
		VECTOR front = VSub(cam.GetTargetPos(), cam.GetPos());
		front.y = 0.0f;
		front = VNorm(front);
		VECTOR left = front;
		std::swap(left.x, left.z);
		left.x = -left.x;
		animCtrl_->Play((int)STATE::AVOID, false, 0.0f, -1.0f, 0.001f);
		avoidDir_ = Utility::VECTOR_ZERO;
		if (keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_UP, KeyConfig::JOYPAD_NO::PAD1, controlType_))
		{
			avoidDir_ = VAdd(avoidDir_, front);
		}
		if (keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_DOWN, KeyConfig::JOYPAD_NO::PAD1, controlType_))
		{
			avoidDir_ = VAdd(avoidDir_, VScale(front, -1));
		}
		if (keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_RIGHT, KeyConfig::JOYPAD_NO::PAD1, controlType_))
		{
			avoidDir_ = VAdd(avoidDir_, VScale(left, -1));
		}
		if (keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_LEFT, KeyConfig::JOYPAD_NO::PAD1, controlType_))
		{
			avoidDir_ = VAdd(avoidDir_, left);
		}

		if (controlType_ == KeyConfig::TYPE::KEYBORD_MOUSE)
		{
			avoidDir_ = VNorm(avoidDir_);
			return;
		}
		auto stick2D = (keyIns_.GetKnockLStickSize(KeyConfig::JOYPAD_NO::PAD1));
		if (stick2D.x != 0.0f || stick2D.y != 0.0f)
		{
			auto stick3D = Utility::Normalize(stick2D);
			avoidDir_ = VAdd(VScale(front, stick3D.y * -1), VScale(left, stick3D.x * -1));
		}
	}
	else
	{
		avoidDir_ = VSub(cam.GetTargetPos(), cam.GetPos());
		avoidDir_.y = 0.0f;
		avoidDir_ = VNorm(avoidDir_);
	}
	animCtrl_->Play((int)STATE::AVOID, false, 0.0f, -1.0f, 0.1f, false, true);
}


void GamePlayer::StateChangeAttack(void)
{
	CreateShot();
	attackDeley_ = ATTACK_DELEY;
	animCtrl_->Play((int)STATE::ATTACK, false, 0.0f, -1.0f, 0.1f, false, true);
	stateUpdate_ = std::bind(&GamePlayer::StateUpdateAttack, this);
}

void GamePlayer::StateChangeDamage(void)
{
	damageTime_ = DAMAGE_TIME;
	damageInvincibleTime_ = DAMAGE_INVINCIBLE_TIME;
	animCtrl_->Play((int)STATE::DAMAGE, true, 0.0f, -1.0f, 0.1f);
	stateUpdate_ = std::bind(&GamePlayer::StateUpdateDamage, this);
}

void GamePlayer::StateChangeDead(void)
{
	stateUpdate_ = std::bind(&GamePlayer::StateUpdateDead, this);
}

void GamePlayer::StateUpdateIdle(void)
{
	if (IsPushMoveKey())
	{
		ChangeState(STATE::MOVE);
	}
	else if (keyIns_.IsTrgDown(KeyConfig::CONTROL_TYPE::PLAYER_AVOID, KeyConfig::JOYPAD_NO::PAD1, controlType_) && avoidCoolTime_ < 0.0f)
	{
		ChangeState(STATE::AVOID);
	}
	else if (keyIns_.IsTrgDown(KeyConfig::CONTROL_TYPE::PLAYER_JUMP, KeyConfig::JOYPAD_NO::PAD1, controlType_))
	{
		ChangeState(STATE::JUMP);
	}
}

void GamePlayer::StateUpdateMove(void)
{
	PlayerMove();
	if (keyIns_.IsTrgDown(KeyConfig::CONTROL_TYPE::PLAYER_AVOID, KeyConfig::JOYPAD_NO::PAD1, controlType_) && avoidCoolTime_ < 0.0f)
	{
		StopFootSound();
		ChangeState(STATE::AVOID);
	}
	else if (keyIns_.IsTrgDown(KeyConfig::CONTROL_TYPE::PLAYER_JUMP, KeyConfig::JOYPAD_NO::PAD1, controlType_))
	{
		StopFootSound();
		ChangeState(STATE::JUMP);
	}
	else if (!IsPushMoveKey())
	{
		StopFootSound();
		ChangeState(STATE::IDLE);
	}
	if (IsPlayFootSound())
	{
		return;
	}
	PlayFootSound();
}

void GamePlayer::StateUpdateJump(void)
{
	PlayerMove();
	if (gravity_->GetState() == Gravity::STATE::NONE)
	{
		ChangeState(STATE::IDLE);
	}
}

void GamePlayer::StateUpdateAvoid(void)
{
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();
	avoidTime_ -= deltaTime;
	rimPow_ = Utility::Lerp(RIM_MAX_POW, RIM_MIN_POW, 1.0f - (avoidTime_ / avoidSaccessTime_));
	pointLight_->SetRadiusPercent((avoidTime_ / avoidSaccessTime_));
	transform_->pos = VAdd(transform_->pos, VScale(avoidDir_, AVOID_DISTANCE / ((1.0f / deltaTime) * AVOID_TIME)));
	if (avoidTime_ <= 0.0f)
	{
		if (!isAvoidSaccess_)
		{
			avoidCoolTime_ = AVOID_COOL_TIME;
		}
		isAvoidSaccess_ = false;
		avoidSaccessTime_ = -1.0f;
		rimPow_ = RIM_MIN_POW;
		if (KeyConfig::GetInstance().IsTrgDown(KeyConfig::CONTROL_TYPE::PLAYER_AVOID, KeyConfig::JOYPAD_NO::PAD1, controlType_) && avoidCoolTime_ < 0.0f)
		{
			ChangeState(STATE::AVOID, true);
			return;
		}
		animCtrl_->Play((int)STATE::IDLE, true, 0.0f, -1.0f, 0.01f);
		ChangeState(STATE::IDLE);
		return;
	}
}

void GamePlayer::StateUpdateAttack(void)
{
	PlayerMove();
	if (animCtrl_->IsEnd())
	{
		animCtrl_->Play((int)STATE::IDLE, true, 0.0f, -1.0f, -1.0f);
		ChangeState(STATE::IDLE);
	}
}

void GamePlayer::StateUpdateDamage(void)
{
	if (damageTime_ < 0.0f)
	{
		if (isDeath_)
		{
			ChangeState(STATE::DEAD);
		}
		else
		{
			material_->SetConstBufPS(0, Utility::COLOR_F2FLOAT4(DEFAULT_COLOR_TIMES));
			ChangeState(STATE::IDLE);
		}
	}
	else
	{
		transform_->pos = VAdd(transform_->pos, VScale(damageDir_, DAMAGE_SPEED));
	}
}

void GamePlayer::StateUpdateDead(void)
{
}
