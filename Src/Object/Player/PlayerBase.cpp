#include "../../Manager/DataBank.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/SoundManager.h"
#include "../../Manager/Camera.h"
#include "../Common/Gravity.h"
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
	gravity_ = std::make_unique<Gravity>();
	gravity_->ChengeState(Gravity::STATE::NONE);
	gravity_->SetDir(Utility::DIR_D);
	gravity_->ChengeState(Gravity::STATE::JUMP);
	controlType_ = DataBank::GetInstance().GetControlType();
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
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();
	avoidCoolTime_ -= deltaTime;
	attackDeley_ -= deltaTime;
	damageTime_ -= deltaTime;
	damageInvincibleTime_ -= deltaTime;
	stateUpdate_();
	gravity_->Update();
	AplayGravity();
	MoveLimit();

	if (keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_ATTACK, KeyConfig::JOYPAD_NO::PAD1, controlType_) && attackDeley_ < 0.0f)
	{
		ChangeState(STATE::ATTACK);
	}
	for (auto& shot : shots_)
	{
		shot->Update();
		if (shot->IsDead())shot = nullptr;
	}
	Utility::EraseVectorAllay(shots_);
	
}

void PlayerBase::Draw(void)
{
	int color = (state_ == STATE::DAMAGE) ? GetColor(255, 0, 0) : GetColor(0, 255, 0);
	DrawSphere3D(transform_->pos, RADIUS, 16, color,GetColor(255,0,0),true);
	for (auto& shot : shots_)
	{
		shot->Draw();
	}
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

void PlayerBase::Damage(float damage,VECTOR dir)
{
	if (ChangeState(STATE::DAMAGE))
	{
		hp_ -= damage;
		damageDir_ = dir;
		gravity_->ChengeState(Gravity::STATE::JUMP);
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
	VECTOR front = VSub(cam.GetTargetPos(), cam.GetPos());
	front.y = 0.0f;
	front = VNorm(front);
	VECTOR left = front;
	std::swap(left.x, left.z);
	left.x = -left.x;
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
	auto stick2D = (keyIns_.GetKnockLStickSize(KeyConfig::JOYPAD_NO::PAD1));
	if (stick2D.x == 0.0f && stick2D.y == 0.0f)
	{
		return;
	}
	auto stick3D = Utility::Normalize(stick2D);
	transform_->pos = VAdd(transform_->pos, VScale(front, stick3D.y * MOVE_SPEED * -1));
	transform_->pos = VAdd(transform_->pos, VScale(left, stick3D.x * MOVE_SPEED * -1));
}

void PlayerBase::MoveLimit(void)
{
	auto& pos = transform_->pos;
	VECTOR prePos = pos;
	pos.x = std::max(std::min(MOVE_LIMIT_MAX.x, pos.x), MOVE_LIMIT_MIN.x);
	pos.y = std::max(std::min(MOVE_LIMIT_MAX.y, pos.y), MOVE_LIMIT_MIN.y);
	pos.z = std::max(std::min(MOVE_LIMIT_MAX.z, pos.z), MOVE_LIMIT_MIN.z);
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
	stateChanges_[(STATE::CHARGE)] = std::bind(&PlayerBase::StateChangeCharge, this);
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

void PlayerBase::StateChangeIdle(void)
{
	stateUpdate_ = std::bind(&PlayerBase::StateUpdateIdle, this);
}

void PlayerBase::StateChangeMove(void)
{
	stateUpdate_ = std::bind(&PlayerBase::StateUpdateMove, this);
}

void PlayerBase::StateChangeJump(void)
{
	gravity_->ChengeState(Gravity::STATE::JUMP);
	gravity_->SetInitPower(JUMP_POW);
	stateUpdate_ = std::bind(&PlayerBase::StateUpdateJump, this);
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
}

void PlayerBase::StateChangeCharge(void)
{
	stateUpdate_ = std::bind(&PlayerBase::StateUpdateCharge, this);
}

void PlayerBase::StateChangeAttack(void)
{
	CreateShot();
	attackDeley_ = ATTACK_DELEY;
	stateUpdate_ = std::bind(&PlayerBase::StateUpdateAttack, this);
}

void PlayerBase::StateChangeDamage(void)
{
	damageTime_ = DAMAGE_TIME;
	damageInvincibleTime_ = DAMAGE_INVINCIBLE_TIME;
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

void PlayerBase::StateUpdateCharge(void)
{
	PlayerMove();
	if (keyIns_.IsTrgDown(KeyConfig::CONTROL_TYPE::PLAYER_ATTACK, KeyConfig::JOYPAD_NO::PAD1, controlType_))
	{
		ChangeState(STATE::ATTACK);
	}
}

void PlayerBase::StateUpdateAttack(void)
{
	PlayerMove();
	ChangeState(STATE::IDLE);
}

void PlayerBase::StateUpdateDamage(void)
{
	if (damageTime_ < 0.0f)
	{
		if (hp_ < 0.0f)
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
