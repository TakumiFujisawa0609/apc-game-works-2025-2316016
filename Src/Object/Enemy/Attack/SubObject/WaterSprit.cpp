#include <memory>
#include "../../Utility/Utility.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/DrawTranslucentManager.h"
#include "../../Renderer/ModelMaterial.h"
#include "../../Renderer/ModelRenderer.h"
#include "../../../Common/EffectController.h"
#include "../../../Common/Transform.h"
#include "../../../Stage/Stage.h"
#include "WaterSprit.h"

WaterSprit::WaterSprit(VECTOR direction, VECTOR startPos , float speed)
{
	transform_ = std::make_shared<Transform>();
	transform_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::WATER_SPRIT_MODEL));
	transform_->pos = startPos;
	transform_->Update();
	material_ = std::make_unique<ModelMaterial>(
		"WaterSplitVS.cso", 2,
		"WaterSplitPS.cso", 2
	);
	material_->AddConstBufVS({ 1.0f, 10.0f,0.0f,0.0f });
	material_->AddConstBufVS({ startPos_.x, startPos_.y, startPos_.z,0.0f });
	material_->AddConstBufPS({ time_,TIME_SCALE, 0.0f, 0.0f });
	//material_->SetTextureBuf(3, ResourceManager::GetInstance().Load(ResourceManager::SRC::NOISE).handleId_);
	renderer_ = std::make_shared<ModelRenderer>(
		transform_->modelId, *material_
	);
	speed_ = speed;
	startPos_ = startPos;
	dir_ = VNorm(direction);
	size_ = INIT_SIZE;
	time_ = ALIVE_TIME;
	effect_ = std::make_unique<EffectController>();
	effect_->Add(ResourceManager::GetInstance().Load(ResourceManager::SRC::WATER_SPLIT).handleId_, EffectController::EFF_TYPE::WATER_SPLIT);
	float size = size_ * EFFECT_INIT_SIZE;
	//effectHandle_ =effect_->Play(EffectController::EFF_TYPE::WATER_SPLIT, transform_->pos, Quaternion::Identity(), VGet(size, size, size), true,1.0f );
	isEnd_ = false;
	rot_ =0.0f;
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
	rot_ += Utility::Deg2RadF(180.0f) * deltaTime;
	time_ -= deltaTime;
	transform_->pos = VAdd(transform_->pos, VScale(dir_, speed_));
	transform_->quaRot = Quaternion(VGet(0.0f, rot_, 0.0f));
	transform_->Update();
	if (time_ < 0.0f || Utility::Distance(startPos_ , transform_->pos) > Stage::RADIUS)
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
	auto pos = transform_->pos;
	material_->SetConstBufVS(1,{pos.x, pos.y, pos.z ,0.0f });
	material_->SetConstBufPS(0,{ time_, TIME_SCALE,0.0f, 0.0f });
}

void WaterSprit::Draw(void)
{
	//Utility::DrawCircle3DXZ(transform_->pos, size_, 16, GetColor(0, 0, 255), true);
	DrawTranslucentManager::GetInstance().Add(transform_,renderer_);
	//renderer_->Draw();
}

