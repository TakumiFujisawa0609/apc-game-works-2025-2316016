#include "../../Manager/KeyConfig.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/Camera.h"
#include "../Common/Gravity.h"
#include "PlayerBase.h"

PlayerBase::PlayerBase(int playerNum)
{
	playerNum_ = playerNum;
	hp_ = MAX_HP;
	transform_ = std::make_unique<Transform>();
	gravity_ = std::make_unique<Gravity>();
	gravity_->SetDir(VGet(0.0f, -1.0f, 0.0f));
	gravity_->Init();
}

PlayerBase::~PlayerBase(void)
{
}

void PlayerBase::Init(void)
{
}

void PlayerBase::Update(void)
{
	ChengeGravityDir();
	PlayerMove();
	ApplyGravity();
	MoveLimit();
}

void PlayerBase::Draw(void)
{
	int color = (playerNum_ == 0) ? GetColor(0, 0, 255) : GetColor(0, 255, 0);
	DrawSphere3D(transform_->pos, 10.0f, 16, color,GetColor(255,0,0),true);
	DrawFormatString(0, playerNum_ * 16, 0xffffff, "%d,%d,%d", (int)transform_->pos.x, (int)transform_->pos.y, (int)transform_->pos.z);
}

void PlayerBase::ChengeGravityDir(void)
{
}

void PlayerBase::PlayerMove(void)
{
	KeyConfig& keyIns = KeyConfig::GetInstance();
	SceneManager& sceneIns = SceneManager::GetInstance();
	Camera& cam = sceneIns.GetCamera();
	VECTOR front = VSub(cam.GetTargetPos(), cam.GetPos());
	front.y = 0.0f;
	front = VNorm(front);
	VECTOR left = front;
	std::swap(left.x, left.z);
	left.x = -left.x;
	//if (Utility::GetSign(left.x) == Utility::GetSign(left.z))
	//{
	//	left.z = -left.z;
	//}
	//else
	//{
	//	left.x = -left.x;
	//}
	if (playerNum_ == 0)
	{
		if (keyIns.IsNew(KeyConfig::CONTROL_TYPE::PLAYER1_MOVE_UP, KeyConfig::JOYPAD_NO::PAD1))
		{
			transform_->pos = VAdd(transform_->pos, VScale(front, MOVE_SPEED));
		}
		if (keyIns.IsNew(KeyConfig::CONTROL_TYPE::PLAYER1_MOVE_DOWN, KeyConfig::JOYPAD_NO::PAD1))
		{
			transform_->pos = VAdd(transform_->pos, VScale(VScale(front,-1), MOVE_SPEED));
		}
		if (keyIns.IsNew(KeyConfig::CONTROL_TYPE::PLAYER1_MOVE_RIGHT, KeyConfig::JOYPAD_NO::PAD1))
		{
			transform_->pos = VAdd(transform_->pos, VScale(VScale(left, -1), MOVE_SPEED));
		}
		if (keyIns.IsNew(KeyConfig::CONTROL_TYPE::PLAYER1_MOVE_LEFT, KeyConfig::JOYPAD_NO::PAD1))
		{
			transform_->pos = VAdd(transform_->pos, VScale(left, MOVE_SPEED));
		}
	}
	else
	{
		if (keyIns.IsNew(KeyConfig::CONTROL_TYPE::PLAYER2_MOVE_UP, KeyConfig::JOYPAD_NO::PAD1))
		{
			transform_->pos = VAdd(transform_->pos, VScale(front, MOVE_SPEED));
		}
		if (keyIns.IsNew(KeyConfig::CONTROL_TYPE::PLAYER2_MOVE_DOWN, KeyConfig::JOYPAD_NO::PAD1))
		{
			transform_->pos = VAdd(transform_->pos, VScale(VScale(front, -1), MOVE_SPEED));
		}
		if (keyIns.IsNew(KeyConfig::CONTROL_TYPE::PLAYER2_MOVE_RIGHT, KeyConfig::JOYPAD_NO::PAD1))
		{
			transform_->pos = VAdd(transform_->pos, VScale(VScale(left, -1), MOVE_SPEED));
		}
		if (keyIns.IsNew(KeyConfig::CONTROL_TYPE::PLAYER2_MOVE_LEFT, KeyConfig::JOYPAD_NO::PAD1))
		{
			transform_->pos = VAdd(transform_->pos, VScale(left, MOVE_SPEED));
		}

	}
	if (keyIns.IsTrgDown((playerNum_ == 0) ? KeyConfig::CONTROL_TYPE::PLAYER1_JUMP : KeyConfig::CONTROL_TYPE::PLAYER2_JUMP, KeyConfig::JOYPAD_NO::PAD1))
	{
		Jump();
	}
}

void PlayerBase::Jump(void)
{
	if (place_ == PLACE::LAND)
	{
		gravity_->ChengeState(Gravity::STATE::JUMP);
		gravity_->SetInitPower(JUMP_POWER);
		place_ = PLACE::AIR;
	}
}

void PlayerBase::ApplyGravity(void)
{
	gravity_->Update();
	transform_->pos = VAdd(transform_->pos, VScale(gravity_->GetDir(), gravity_->GetPower()));
}

void PlayerBase::MoveLimit(void)
{
	auto& pos = transform_->pos;
	VECTOR prePos = pos;
	pos.x = std::max(std::min(MOVE_LIMIT_MAX.x, pos.x), MOVE_LIMIT_MIN.x);
	pos.y = std::max(std::min(MOVE_LIMIT_MAX.y, pos.y), MOVE_LIMIT_MIN.y);
	pos.z = std::max(std::min(MOVE_LIMIT_MAX.z, pos.z), MOVE_LIMIT_MIN.z);

	if (pos.x != prePos.x )
	{
		if (pos.x < prePos.x)
		{
			if (Utility::Equals(gravity_->GetDir(), Utility::DIR_R))
			{
				// 位置が制限に引っかかった場合
				gravity_->ChengeState(Gravity::STATE::NONE);
				place_ = PLACE::LAND;
			}
		}
		else
		{
			if (Utility::Equals(gravity_->GetDir(), Utility::DIR_L))
			{
				// 位置が制限に引っかかった場合
				gravity_->ChengeState(Gravity::STATE::NONE);
				place_ = PLACE::LAND;
			}
		}

	}
	if (pos.y != prePos.y )
	{
		if (pos.y < prePos.y)
		{
			if (Utility::Equals(gravity_->GetDir(), Utility::DIR_U))
			{
				// 位置が制限に引っかかった場合
				gravity_->ChengeState(Gravity::STATE::NONE);
				place_ = PLACE::LAND;
			}
		}
		else
		{
			if (Utility::Equals(gravity_->GetDir(), Utility::DIR_D))
			{
				// 位置が制限に引っかかった場合
				gravity_->ChengeState(Gravity::STATE::NONE);
				place_ = PLACE::LAND;
			}
		}

	}
	if (pos.z != prePos.z )
	{
		if (pos.z < prePos.z)
		{
			if (Utility::Equals(gravity_->GetDir(), Utility::DIR_F))
			{
				// 位置が制限に引っかかった場合
				gravity_->ChengeState(Gravity::STATE::NONE);
				place_ = PLACE::LAND;
			}
		}
		else
		{
			if (Utility::Equals(gravity_->GetDir(), Utility::DIR_B))
			{
				// 位置が制限に引っかかった場合
				gravity_->ChengeState(Gravity::STATE::NONE);
				place_ = PLACE::LAND;
			}
		}

	}
}
