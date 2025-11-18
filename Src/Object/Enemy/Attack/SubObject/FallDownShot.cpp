#include <memory>
#include "../../Utility/Utility.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Renderer/ModelMaterial.h"
#include "../../Renderer/ModelRenderer.h"
#include "../../../Common/Transform.h"
#include "../../../Common/EffectController.h"
#include "../../../Common/Geometry/Sphere.h"
#include "../../../Player/PlayerBase.h"
#include "../../../Stage/Stage.h"
#include "FallDownShot.h"

FallDownShot::FallDownShot(void)
{
	transform_ = std::make_shared<Transform>();
	transform_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::CHICKIN));
	transform_->pos = InitPos();
	transform_->Update();
	speed_ = InitSpeed();
	initY = transform_->pos.y;
	state_ = STATE::FALL;
	blastTime_ = 0.0f;
	color_ = { 1.0f,0.0f,0.0f };
	damage_ = DAMAGE;
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
			std::unique_ptr<Geometry>geo = std::make_unique<Sphere>(transform_->pos, radius_);
			MakeCollider(Collider::TAG::ENEMY_ATTACK, std::move(geo), { Collider::TAG::ENEMY,Collider::TAG::ENEMY_ATTACK });
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
	if (state_ == STATE::BLAST)
	{
		DrawSphere3D(pos, radius_, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), true);
	}
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
	//pos.x = PlayerBase::MOVE_LIMIT_MIN.x + GetRand(static_cast<int>(PlayerBase::MOVE_LIMIT_MAX.x - PlayerBase::MOVE_LIMIT_MIN.x));
	//pos.y = PlayerBase::MOVE_LIMIT_MAX.y;
	//pos.z = PlayerBase::MOVE_LIMIT_MIN.z + GetRand(static_cast<int>(PlayerBase::MOVE_LIMIT_MAX.z - PlayerBase::MOVE_LIMIT_MIN.z));
	int deg = GetRand(360);
	float rad = Utility::Deg2RadF(static_cast<float>(deg));
	int range = GetRand(static_cast<int>(Stage::RADIUS));
	pos.x = std::sinf(rad) * range;
	pos.y = PlayerBase::MOVE_LIMIT_MAX.y;
	range = GetRand(static_cast<int>(Stage::RADIUS));
	pos.z = std::cosf(rad) * range;
	return pos;
}
