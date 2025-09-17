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
}

PlayerBase::~PlayerBase(void)
{
}

void PlayerBase::Init(void)
{
}

void PlayerBase::Update(void)
{
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
	if (playerNum_ == 0)
	{
		if (keyIns.IsNew(KeyConfig::CONTROL_TYPE::PLAYER1_MOVE_UP, KeyConfig::JOYPAD_NO::PAD1))
		{
			transform_->pos = VAdd(transform_->pos, VScale(VGet(0.0f,0.0f,1.0f), MOVE_SPEED));
		}
		if (keyIns.IsNew(KeyConfig::CONTROL_TYPE::PLAYER1_MOVE_DOWN, KeyConfig::JOYPAD_NO::PAD1))
		{
			transform_->pos = VAdd(transform_->pos, VScale(VGet(0.0f,0.0f,-1.0f), MOVE_SPEED));
		}
		if (keyIns.IsNew(KeyConfig::CONTROL_TYPE::PLAYER1_MOVE_RIGHT, KeyConfig::JOYPAD_NO::PAD1))
		{
			transform_->pos = VAdd(transform_->pos, VScale(VGet(1.0f,0.0f,0.0f), MOVE_SPEED));
		}
		if (keyIns.IsNew(KeyConfig::CONTROL_TYPE::PLAYER1_MOVE_LEFT, KeyConfig::JOYPAD_NO::PAD1))
		{
			transform_->pos = VAdd(transform_->pos, VScale(VGet(-1.0f,0.0f,0.0f), MOVE_SPEED));
		}
	}
	else
	{
		if (keyIns.IsNew(KeyConfig::CONTROL_TYPE::PLAYER2_MOVE_UP, KeyConfig::JOYPAD_NO::PAD1))
		{
			transform_->pos = VAdd(transform_->pos, VScale(VGet(0.0f, 0.0f, 1.0f), MOVE_SPEED));
		}
		if (keyIns.IsNew(KeyConfig::CONTROL_TYPE::PLAYER2_MOVE_DOWN, KeyConfig::JOYPAD_NO::PAD1))
		{
			transform_->pos = VAdd(transform_->pos, VScale(VGet(0.0f, 0.0f, -1.0f), MOVE_SPEED));
		}
		if (keyIns.IsNew(KeyConfig::CONTROL_TYPE::PLAYER2_MOVE_RIGHT, KeyConfig::JOYPAD_NO::PAD1))
		{
			transform_->pos = VAdd(transform_->pos, VScale(VGet(1.0f, 0.0f, 0.0f), MOVE_SPEED));
		}
		if (keyIns.IsNew(KeyConfig::CONTROL_TYPE::PLAYER2_MOVE_LEFT, KeyConfig::JOYPAD_NO::PAD1))
		{
			transform_->pos = VAdd(transform_->pos, VScale(VGet(-1.0f, 0.0f, 0.0f), MOVE_SPEED));
		}

	}
}

void PlayerBase::ApplyGravity(void)
{
}

void PlayerBase::MoveLimit(void)
{
	auto& pos = transform_->pos;
	VECTOR prePos = pos;
	pos.x = std::max(std::min(MOVE_LIMIT_MAX.x, pos.x), MOVE_LIMIT_MIN.x);
	pos.y = std::max(std::min(MOVE_LIMIT_MAX.y, pos.y), MOVE_LIMIT_MIN.y);
	pos.z = std::max(std::min(MOVE_LIMIT_MAX.z, pos.z), MOVE_LIMIT_MIN.z);
	if (pos.x != prePos.x || pos.y != prePos.y || pos.z != prePos.z)
	{
		// à íuÇ™êßå¿Ç…à¯Ç¡Ç©Ç©Ç¡ÇΩèÍçá

	}
}
