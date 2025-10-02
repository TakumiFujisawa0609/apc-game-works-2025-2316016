#include "../../Utility/Utility.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Common/Transform.h"
#include "../../Player/PlayerBase.h"
#include "FallDownShot.h"

FallDownShot::FallDownShot(void)
{
	transform_ = std::make_unique<Transform>();
	transform_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ENEMY));
	transform_->pos = InitPos();
	transform_->Update();
	speed_ = InitSpeed();
	initY = transform_->pos.y;
	state_ = STATE::FALL;
	blastTime_ = 0.0f;
	color_ = { 1.0f,0.0f,0.0f };
}

FallDownShot::~FallDownShot(void)
{
}

void FallDownShot::Init(void)
{
}

void FallDownShot::Update(void)
{
	if (state_ == STATE::FALL)
	{
		transform_->pos.y -= speed_;
		color_ = Utility::Lerp(GetColorF(1.0f, 1.0f, 0.0f,1.0f), GetColorF(1.0f, 0.0f, 0.0f,1.0f),1.0f - (transform_->pos.y - PlayerBase::MOVE_LIMIT_MIN.y)/(initY - PlayerBase::MOVE_LIMIT_MIN.y));
		if (transform_->pos.y < PlayerBase::MOVE_LIMIT_MIN.y)
		{
			state_ = STATE::BLAST;
			blastTime_ = BLAST_TIME;
		}
		transform_->Update();
	}
	else if (state_ == STATE::BLAST)
	{
		blastTime_ -= SceneManager::GetInstance().GetDeltaTime();
		if (blastTime_ < 0.0f)
		{
			state_ = STATE::DEAD;
		}
	}
	radius_ = (transform_->pos.y - PlayerBase::MOVE_LIMIT_MIN.y) /((initY - PlayerBase::MOVE_LIMIT_MIN.y) / (RADIUS_MAX - RADIUS_MIN)) + RADIUS_MIN;
}

void FallDownShot::Draw(void)
{
	MV1DrawModel(transform_->modelId);
	VECTOR pos = transform_->pos;
	pos.y = 0.0f;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, ALPHA);
	Utility::DrawCircle3DXZ(pos,radius_, VERTEX_NUM, GetColor(color_.r * 255,color_.g * 255,color_.b*255), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void FallDownShot::Hit(void)
{
	state_ = STATE::BLAST;
}

float FallDownShot::InitSpeed(void)
{
	return GetRand(static_cast<int>((FAST_SPEED - SLOW_SPEED) * 10)) / 10 + SLOW_SPEED;
}

VECTOR FallDownShot::InitPos(void)
{
	VECTOR pos = {};
	pos.x = PlayerBase::MOVE_LIMIT_MIN.x + GetRand(static_cast<int>(PlayerBase::MOVE_LIMIT_MAX.x - PlayerBase::MOVE_LIMIT_MIN.x));
	pos.y = PlayerBase::MOVE_LIMIT_MAX.y;
	pos.z = PlayerBase::MOVE_LIMIT_MIN.z + GetRand(static_cast<int>(PlayerBase::MOVE_LIMIT_MAX.z - PlayerBase::MOVE_LIMIT_MIN.z));
	return pos;
}
