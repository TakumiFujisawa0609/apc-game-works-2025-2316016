#include <memory>
#include "../../../../Manager/ResourceManager.h"
#include "../../../../Manager/SceneManager.h"
#include "../../Renderer/ModelMaterial.h"
#include "../../Renderer/ModelRenderer.h"
#include "../../../Common/Transform.h"
#include "../../../Common/EffectController.h"
#include "../../../Common/Geometry/Sphere.h"
#include "FollowShot.h"

FollowShot::FollowShot(Transform& target, SPEED_TYPE speed, VECTOR startPos) : target_(target)
{
	transform_ = std::make_shared<Transform>();
	transform_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::CHICKIN));
	transform_->pos = startPos;
	time_ = ATTACK_TIME;
	transform_->Update();
	std::unique_ptr<Geometry>geo = std::make_unique<Sphere>(transform_->pos, RADIUS);
	MakeCollider(Collider::TAG::ENEMY_ATTACK, std::move(geo), { Collider::TAG::ENEMY,Collider::TAG::ENEMY_ATTACK });
	speed_ = InitSpeed(speed);
	damage_ = DAMAGE;
}

FollowShot::~FollowShot(void)
{
}

void FollowShot::Init(void)
{
}

void FollowShot::Update(void)
{
	time_ -= SceneManager::GetInstance().GetDeltaTime();
	auto dir = VSub(target_.pos, transform_->pos);
	dir.y = 0.0f;
	transform_->pos = VAdd(transform_->pos, VScale(VNorm(dir), speed_));
	transform_->Update();
	if (time_ < 0.0f)
	{
		state_ = STATE::DEAD;
	}
}

void FollowShot::Draw(void)
{
	MV1DrawModel(transform_->modelId);
}

void FollowShot::Hit(void)
{
	state_ = STATE::BLAST;
}

float FollowShot::InitSpeed(SPEED_TYPE speedType) const
{
	switch (speedType)
	{
	case SPEED_TYPE::SLOW:
		return SLOW_SPEED;
	case SPEED_TYPE::MIDIUM:
		return MIDIUM_SPEED;
	case SPEED_TYPE::FAST:
		return FAST_SPEED;
	case SPEED_TYPE::RANDOM:
		return GetRand(static_cast<int>((FAST_SPEED - SLOW_SPEED)*10))/10 + SLOW_SPEED;
	default:
		return 0.0f;
	}
}
