#include "../../Manager/KeyConfig.h"
#include "../../Manager/SceneManager.h"
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
	transform_ = std::make_unique<Transform>();
	transform_->pos = MOVE_LIMIT_MIN;
	SetupStateChange();
	ChengeState(STATE::IDLE,true);
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
	stateUpdate_();
	MoveLimit();

	if (keyIns_.IsTrgDown(KeyConfig::CONTROL_TYPE::PLAYER_ATTACK, KeyConfig::JOYPAD_NO::PAD1) && attackDeley_ < 0.0f)
	{
		ChengeState(STATE::ATTACK);
	}
	for (auto& shot : shots_)
	{
		shot->Update();
	}
}

void PlayerBase::Draw(void)
{
	int color = (playerNum_ == 0) ? GetColor(0, 0, 255) : GetColor(0, 255, 0);
	DrawSphere3D(transform_->pos, 10.0f, 16, color,GetColor(255,0,0),true);
	for (auto& shot : shots_)
	{
		shot->Draw();
	}
}

void PlayerBase::ChengeState(STATE state, bool isAbsolute )
{
	if (state_ != state || isAbsolute == true)
	{
		state_ = state;
		stateChanges_[state_]();
	}
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
	if (playerNum_ == 0)
	{
		if (keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_UP, KeyConfig::JOYPAD_NO::PAD1))
		{
			transform_->pos = VAdd(transform_->pos, VScale(front, MOVE_SPEED));
		}
		if (keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_DOWN, KeyConfig::JOYPAD_NO::PAD1))
		{
			transform_->pos = VAdd(transform_->pos, VScale(VScale(front,-1), MOVE_SPEED));
		}
		if (keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_RIGHT, KeyConfig::JOYPAD_NO::PAD1))
		{
			transform_->pos = VAdd(transform_->pos, VScale(VScale(left, -1), MOVE_SPEED));
		}
		if (keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_LEFT, KeyConfig::JOYPAD_NO::PAD1))
		{
			transform_->pos = VAdd(transform_->pos, VScale(left, MOVE_SPEED));
		}
	}
	//else
	//{
	//	if (keyIns.IsNew(KeyConfig::CONTROL_TYPE::PLAYER2_MOVE_UP, KeyConfig::JOYPAD_NO::PAD1))
	//	{
	//		transform_->pos = VAdd(transform_->pos, VScale(front, MOVE_SPEED));
	//	}
	//	if (keyIns.IsNew(KeyConfig::CONTROL_TYPE::PLAYER2_MOVE_DOWN, KeyConfig::JOYPAD_NO::PAD1))
	//	{
	//		transform_->pos = VAdd(transform_->pos, VScale(VScale(front, -1), MOVE_SPEED));
	//	}
	//	if (keyIns.IsNew(KeyConfig::CONTROL_TYPE::PLAYER2_MOVE_RIGHT, KeyConfig::JOYPAD_NO::PAD1))
	//	{
	//		transform_->pos = VAdd(transform_->pos, VScale(VScale(left, -1), MOVE_SPEED));
	//	}
	//	if (keyIns.IsNew(KeyConfig::CONTROL_TYPE::PLAYER2_MOVE_LEFT, KeyConfig::JOYPAD_NO::PAD1))
	//	{
	//		transform_->pos = VAdd(transform_->pos, VScale(left, MOVE_SPEED));
	//	}
	//}
}

void PlayerBase::MoveLimit(void)
{
	auto& pos = transform_->pos;
	VECTOR prePos = pos;
	pos.x = std::max(std::min(MOVE_LIMIT_MAX.x, pos.x), MOVE_LIMIT_MIN.x);
	pos.y = std::max(std::min(MOVE_LIMIT_MAX.y, pos.y), MOVE_LIMIT_MIN.y);
	pos.z = std::max(std::min(MOVE_LIMIT_MAX.z, pos.z), MOVE_LIMIT_MIN.z);

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
	if (keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_DOWN, KeyConfig::JOYPAD_NO::PAD1)
		|| keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_UP, KeyConfig::JOYPAD_NO::PAD1)
		|| keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_LEFT, KeyConfig::JOYPAD_NO::PAD1)
		|| keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_RIGHT, KeyConfig::JOYPAD_NO::PAD1))
	{
		return true;
	}
	return false;
}

void PlayerBase::CreateShot(void)
{
	std::unique_ptr<PlayerShot> shot = std::make_unique<PlayerShot>(transform_->pos,SceneManager::GetInstance().GetCamera().GetTargetPos());
	shots_.push_back(std::move(shot));
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
		if (keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_UP, KeyConfig::JOYPAD_NO::PAD1))
		{
			avoidDir_ = front;
		}
		if (keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_DOWN, KeyConfig::JOYPAD_NO::PAD1))
		{
			avoidDir_ = VScale(front, -1);
		}
		if (keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_RIGHT, KeyConfig::JOYPAD_NO::PAD1))
		{
			avoidDir_ =  VScale(left, -1);
		}
		if (keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_MOVE_LEFT, KeyConfig::JOYPAD_NO::PAD1))
		{
			avoidDir_ = left;
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
		ChengeState(STATE::MOVE);
	}
	else if (keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_AVOID, KeyConfig::JOYPAD_NO::PAD1) && avoidCoolTime_ < 0.0f)
	{
		ChengeState(STATE::AVOID);
	}
	else if (keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_JUMP, KeyConfig::JOYPAD_NO::PAD1))
	{
		ChangeState(STATE::JUMP);
	}
}

void PlayerBase::StateUpdateMove(void)
{
	PlayerMove();
	if (keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_AVOID, KeyConfig::JOYPAD_NO::PAD1) && avoidCoolTime_ < 0.0f)
	{
		ChengeState(STATE::AVOID);
	}
	else if (keyIns_.IsNew(KeyConfig::CONTROL_TYPE::PLAYER_JUMP, KeyConfig::JOYPAD_NO::PAD1))
	{
		ChangeState(STATE::JUMP);
	}
	else if (!IsPushMoveKey())
	{
		ChengeState(STATE::IDLE);
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
		ChengeState(STATE::IDLE);
		return;
	}
}

void PlayerBase::StateUpdateCharge(void)
{
	PlayerMove();
	if (keyIns_.IsTrgDown(KeyConfig::CONTROL_TYPE::PLAYER_ATTACK, KeyConfig::JOYPAD_NO::PAD1))
	{
		ChengeState(STATE::ATTACK);
	}
}

void PlayerBase::StateUpdateAttack(void)
{
	PlayerMove();
	ChengeState(STATE::IDLE);
}

void PlayerBase::StateUpdateDamage(void)
{
}

void PlayerBase::StateUpdateDead(void)
{
}
