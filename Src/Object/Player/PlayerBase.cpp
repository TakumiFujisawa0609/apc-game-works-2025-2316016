#include "../../Application.h"
#include "../../Manager/DataBank.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/SoundManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Camera.h"
#include "../Common/Gravity.h"
#include "../Common/Transform.h"
#include "../Common/AnimationController.h"
#include "PlayerShot.h"
#include "PlayerBase.h"

PlayerBase::PlayerBase(int playerNum) :keyIns_(KeyConfig::GetInstance())
{
	playerNum_ = playerNum;
	hp_ = MAX_HP;
	avoidCoolTime_ = 0.0f;
	attackDeley_ = 0.0f;
	damageInvincibleTime_ = 0.0f;
	damageTime_ = 0.0f;
	transform_ = std::make_unique<Transform>();
	transform_->pos = MOVE_LIMIT_MIN;
	transform_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::PLAYER));
	transform_->scl = VGet(SIZE, SIZE, SIZE);
	transform_->Update();
	gravity_ = std::make_unique<Gravity>();
	gravity_->ChengeState(Gravity::STATE::NONE);
	gravity_->SetDir(Utility::DIR_D);
	gravity_->ChengeState(Gravity::STATE::JUMP);
	controlType_ = DataBank::GetInstance().GetControlType();
	isDesth_ = false;
	InitAnimationController();
	SetupStateChange();
	ChangeState(STATE::IDLE,true);
}

PlayerBase::~PlayerBase(void)
{
}

void PlayerBase::Init(void)
{
}

void PlayerBase::Update(void)
{
	if (playerNum_ != 0)
	{
		return;
	}
	prePos_ = transform_->pos;
	//デルタタイムを取得し各種時間関係を更新する
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();
	avoidCoolTime_ -= deltaTime;
	attackDeley_ -= deltaTime;
	damageTime_ -= deltaTime;
	damageInvincibleTime_ -= deltaTime;
	//状態ごとの更新
	stateUpdate_();
	//重力の更新
	gravity_->Update();
	//重力の適用
	AplayGravity();
	//移動制限
	MoveLimit();
	Rotation();
	transform_->Update();
	//攻撃のクールタイム中ではなく攻撃ボタンを押したら攻撃する
	if (keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_ATTACK, KeyConfig::JOYPAD_NO::PAD1, controlType_) && attackDeley_ < 0.0f && state_ != STATE::DAMAGE && state_ != STATE::DEAD)
	{
		if (state_ != STATE::DAMAGE && state_ !=STATE::DEAD)
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
	auto i = animCtrl_->GetPlayType();
}

void PlayerBase::Draw(void)
{
	int color = (state_ == STATE::DAMAGE) ? GetColor(255, 0, 0) : GetColor(0, 255, 0);
	DrawSphere3D(transform_->pos, RADIUS, 16, color,GetColor(255,0,0),true);
	MV1DrawModel(transform_->modelId);
	for (auto& shot : shots_)
	{
		shot->Draw();
	}
}

void PlayerBase::SetPos(VECTOR pos)
{
	transform_->pos = pos;
}

bool PlayerBase::ChangeState(STATE state, bool isAbsolute )
{
	if (state_ != state || isAbsolute == true)
	{
		state_ = state;
		stateChanges_[state_]();
		return true;
	}
	return false;
}

void PlayerBase::Damage(float damage, VECTOR dir)
{
	if (ChangeState(STATE::DAMAGE))
	{
		if (hp_ < 0.0f)
		{
			isDesth_ = true;
		}
		hp_ -= damage;
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

bool PlayerBase::IsDamageHit(void)
{
	//回避状態じゃなく無敵状態じゃなかったらダメージを食らう
	return state_ != STATE::AVOID && damageInvincibleTime_ <0.0f;
}

void PlayerBase::PlayerMove(void)
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
	//キーボードでの移動処理
	if (keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_UP, KeyConfig::JOYPAD_NO::PAD1, controlType_))
	{
		transform_->pos = VAdd(transform_->pos, VScale(front, MOVE_SPEED));
	}
	if (keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_DOWN, KeyConfig::JOYPAD_NO::PAD1, controlType_))
	{
		transform_->pos = VAdd(transform_->pos, VScale(VScale(front, -1), MOVE_SPEED));
	}
	if (keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_RIGHT, KeyConfig::JOYPAD_NO::PAD1, controlType_))
	{
		transform_->pos = VAdd(transform_->pos, VScale(VScale(left, -1), MOVE_SPEED));
	}
	if (keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_LEFT, KeyConfig::JOYPAD_NO::PAD1, controlType_))
	{
		transform_->pos = VAdd(transform_->pos, VScale(left, MOVE_SPEED));
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

void PlayerBase::MoveLimit(void)
{
	auto& pos = transform_->pos;
	VECTOR prePos = pos;
	pos.y = std::max(std::min(MOVE_LIMIT_MAX.y, pos.y), MOVE_LIMIT_MIN.y);
	if (prePos.y < pos.y && state_ != STATE::DAMAGE)
	{
		gravity_->ChengeState(Gravity::STATE::NONE);
	}
}

void PlayerBase::AplayGravity(void)
{
	transform_->pos = VAdd(transform_->pos,VScale(gravity_->GetDir(),gravity_->GetPower()));
}

void PlayerBase::SetupStateChange(void)
{
	stateChanges_[(STATE::IDLE)] = std::bind(&PlayerBase::StateChangeIdle, this);
	stateChanges_[(STATE::MOVE)] = std::bind(&PlayerBase::StateChangeMove, this);
	stateChanges_[(STATE::JUMP)] = std::bind(&PlayerBase::StateChangeJump, this);
	stateChanges_[(STATE::AVOID)] = std::bind(&PlayerBase::StateChangeAvoid, this);
	//stateChanges_[(STATE::CHARGE)] = std::bind(&PlayerBase::StateChangeCharge, this);
	stateChanges_[(STATE::ATTACK)] = std::bind(&PlayerBase::StateChangeAttack, this);
	stateChanges_[(STATE::DAMAGE)] = std::bind(&PlayerBase::StateChangeDamage, this);
	stateChanges_[(STATE::DEAD)] = std::bind(&PlayerBase::StateChangeDead, this);
}

bool PlayerBase::IsPushMoveKey(void)
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

void PlayerBase::CreateShot(void)
{
	std::unique_ptr<PlayerShot> shot = std::make_unique<PlayerShot>(transform_->pos,SceneManager::GetInstance().GetCamera().GetTargetPos());
	shots_.push_back(std::move(shot));
	SoundManager::GetInstance().Play(SoundManager::SRC::PSHOT_THROW, Sound::TIMES::ONCE);
}

void PlayerBase::Rotation(void)
{
	VECTOR dir;
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

void PlayerBase::InitAnimationController(void)
{
	std::string path = Application::PATH_MODEL + "Player/";
	animCtrl_ = std::make_unique<AnimationController>(transform_->modelId);
	animCtrl_->Add((int)STATE::IDLE, path + "Idle.mv1", 60.0f);
	animCtrl_->Add((int)STATE::MOVE, path + "Move.mv1", 120.0f);
	animCtrl_->Add((int)STATE::JUMP, path + "Jump.mv1", 80.0f);
	animCtrl_->Add((int)STATE::AVOID, path + "Avoid2.mv1", 100.0f);
	animCtrl_->Add((int)STATE::ATTACK, path + "Throw.mv1", 120.0f);
	animCtrl_->Add((int)STATE::DAMAGE, path + "Damage.mv1", 120.0f);
	//animCtrl_->Add((int)STATE::DEAD, path + "Falling.mv1", 80.0f);

	animCtrl_->Play((int)STATE::IDLE);
}

void PlayerBase::StateChangeIdle(void)
{
	stateUpdate_ = std::bind(&PlayerBase::StateUpdateIdle, this);
	animCtrl_->Play((int)STATE::IDLE);
}

void PlayerBase::StateChangeMove(void)
{
	stateUpdate_ = std::bind(&PlayerBase::StateUpdateMove, this);
	animCtrl_->Play((int)STATE::MOVE);
}

void PlayerBase::StateChangeJump(void)
{
	gravity_->ChengeState(Gravity::STATE::JUMP);
	gravity_->SetInitPower(JUMP_POW);
	stateUpdate_ = std::bind(&PlayerBase::StateUpdateJump, this);
	animCtrl_->Play((int)STATE::JUMP);
};

void PlayerBase::StateChangeAvoid(void)
{
	stateUpdate_ = std::bind(&PlayerBase::StateUpdateAvoid, this);
	avoidTime_ = AVOID_TIME;
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
		animCtrl_->Play((int)STATE::AVOID);
		if (keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_UP, KeyConfig::JOYPAD_NO::PAD1, controlType_))
		{
			avoidDir_ = front;
		}
		if (keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_DOWN, KeyConfig::JOYPAD_NO::PAD1, controlType_))
		{
			avoidDir_ = VScale(front, -1);
		}
		if (keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_RIGHT, KeyConfig::JOYPAD_NO::PAD1, controlType_))
		{
			avoidDir_ =  VScale(left, -1);
		}
		if (keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_LEFT, KeyConfig::JOYPAD_NO::PAD1, controlType_))
		{
			avoidDir_ = left;
		}

		if (controlType_ == KeyConfig::TYPE::KEYBORD_MOUSE)
		{
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
		avoidDir_ =  VSub(cam.GetTargetPos(),cam.GetPos());
		avoidDir_.y = 0.0f;
		avoidDir_ = VNorm(avoidDir_);
	}
	animCtrl_->Play((int)STATE::AVOID);
}

//void PlayerBase::StateChangeCharge(void)
//{
//	stateUpdate_ = std::bind(&PlayerBase::StateUpdateCharge, this);
//}

void PlayerBase::StateChangeAttack(void)
{
	CreateShot();
	attackDeley_ = ATTACK_DELEY;
	animCtrl_->Play((int)STATE::ATTACK);
	stateUpdate_ = std::bind(&PlayerBase::StateUpdateAttack, this);
}

void PlayerBase::StateChangeDamage(void)
{
	damageTime_ = DAMAGE_TIME;
	damageInvincibleTime_ = DAMAGE_INVINCIBLE_TIME;
	animCtrl_->Play((int)STATE::DAMAGE);
	stateUpdate_ = std::bind(&PlayerBase::StateUpdateDamage, this);
}

void PlayerBase::StateChangeDead(void)
{
	stateUpdate_ = std::bind(&PlayerBase::StateUpdateDead, this);
}

void PlayerBase::StateUpdateIdle(void)
{
	if (IsPushMoveKey())
	{
		ChangeState(STATE::MOVE);
	}
	else if (keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_AVOID, KeyConfig::JOYPAD_NO::PAD1, controlType_) && avoidCoolTime_ < 0.0f)
	{
		ChangeState(STATE::AVOID);
	}
	else if (keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_JUMP, KeyConfig::JOYPAD_NO::PAD1, controlType_))
	{
		ChangeState(STATE::JUMP);
	}
}

void PlayerBase::StateUpdateMove(void)
{
	PlayerMove();
	if (keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_AVOID, KeyConfig::JOYPAD_NO::PAD1, controlType_) && avoidCoolTime_ < 0.0f)
	{
		ChangeState(STATE::AVOID);
	}
	else if (keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_JUMP, KeyConfig::JOYPAD_NO::PAD1, controlType_))
	{
		ChangeState(STATE::JUMP);
	}
	else if (!IsPushMoveKey())
	{
		ChangeState(STATE::IDLE);
	}
}

void PlayerBase::StateUpdateJump(void)
{
	PlayerMove();
	if (gravity_->GetState() == Gravity::STATE::NONE)
	{
		ChangeState(STATE::IDLE);
	}
}

void PlayerBase::StateUpdateAvoid(void)
{
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();
	avoidTime_ -= deltaTime;
	transform_->pos = VAdd(transform_->pos, VScale(avoidDir_, AVOID_DISTANCE / ((1.0f /deltaTime) * AVOID_TIME) ));
	if (avoidTime_ <= 0.0f)
	{
		avoidCoolTime_ = AVOID_COOL_TIME;
		ChangeState(STATE::IDLE);
		return;
	}
}

//void PlayerBase::StateUpdateCharge(void)
//{
//	PlayerMove();
//	if (keyIns_.IsTrgDown(KeyConfig::CONTROL_TYPE::PLAYER_ATTACK, KeyConfig::JOYPAD_NO::PAD1, controlType_))
//	{
//		ChangeState(STATE::ATTACK);
//	}
//}

void PlayerBase::StateUpdateAttack(void)
{
	PlayerMove();
	if (attackDeley_ < 0.0f)
	{
		ChangeState(STATE::IDLE);
	}
}

void PlayerBase::StateUpdateDamage(void)
{
	if (damageTime_ < 0.0f)
	{
		if (isDesth_)
		{
			ChangeState(STATE::DEAD);
		}
		else
		{
			ChangeState(STATE::IDLE);
		}
	}
	else
	{
		transform_->pos = VAdd(transform_->pos, VScale(damageDir_, DAMAGE_SPEED));
	}
}

void PlayerBase::StateUpdateDead(void)
{
}
