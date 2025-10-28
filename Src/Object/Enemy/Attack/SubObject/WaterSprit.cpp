#include <memory>
#include "../../Utility/Utility.h"
#include "../../Manager/ResourceManager.h"
#include "../../../../Manager/SceneManager.h"
#include "../../Renderer/ModelMaterial.h"
#include "../../Renderer/ModelRenderer.h"
#include "../../../Common/EffectController.h"
#include "../../../Common/Transform.h"
#include "../../../Stage/ShockWave.h"
#include "WaterSprit.h"

WaterSprit::WaterSprit(VECTOR direction, VECTOR startPos , float speed)
{
	transform_ = std::make_unique<Transform>();
	transform_->SetModel(-1);
	transform_->pos = startPos;
	transform_->Update();
	speed_ = speed;
	startPos_ = startPos;
	dir_ = VNorm(direction);
	size_ = INIT_SIZE;
	time_ = ALIVE_TIME;
	effect_ = std::make_unique<EffectController>();
	effect_->Add(ResourceManager::GetInstance().Load(ResourceManager::SRC::WATER_SPLIT).handleId_, EffectController::EFF_TYPE::WATER_SPLIT);
	float size = size_ * EFFECT_INIT_SIZE;
	effectHandle_ =effect_->Play(EffectController::EFF_TYPE::WATER_SPLIT, transform_->pos, Quaternion::Identity(), VGet(size, size, size), true, 1.0f);
	isEnd_ = false;
}

WaterSprit::~WaterSprit(void)
{
}

void WaterSprit::Init(void)
{
}

void WaterSprit::Update(void)
{
	auto deltaTime = SceneManager::GetInstance().GetDeltaTime();
	size_ += SECOND_PER_SIZE * deltaTime;
	time_ -= deltaTime;
	transform_->pos = VAdd(transform_->pos, VScale(dir_, speed_));
	transform_->Update();
	if (time_ < 0.0f || Utility::Distance(startPos_ , transform_->pos) > ShockWave::RADIUS)
	{
		isEnd_ = true;
		effect_->Stop(EffectController::EFF_TYPE::WATER_SPLIT, effectHandle_);
		effect_->AllDelete();
	}
	else
	{
		effect_->SetPos(EffectController::EFF_TYPE::WATER_SPLIT, effectHandle_, transform_->pos);
		float size = size_ * EFFECT_INIT_SIZE;
		effect_->SetScale(EffectController::EFF_TYPE::WATER_SPLIT, effectHandle_, VGet(size, size, size));
		effect_->Update();
	}
}

void WaterSprit::Draw(void)
{
	Utility::DrawCircle3DXZ(transform_->pos, size_, 16, GetColor(0, 0, 255), true);
}

