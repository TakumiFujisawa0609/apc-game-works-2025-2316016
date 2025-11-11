#include "../../../../Manager/ResourceManager.h"
#include "../../../../Manager/DrawTranslucentManager.h"
#include "../../../../Manager/SceneManager.h"
#include "../../Renderer/ModelMaterial.h"
#include "../../Renderer/ModelRenderer.h"
#include "../../../Common/Transform.h"
#include "../../../Common/EffectController.h"
#include "CrossLine.h"

CrossLine::CrossLine(VECTOR centerPos, float& rad, float initRad, int num) : centerPos_(centerPos), radian_(rad)
{
	time_ = 0.0f;
	transform_ = std::make_shared<Transform>();
	transform_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::FIRE_MODEL));
	transform_->scl = VGet(SIZE_XZ, SIZE_Y, SIZE_XZ);
	transform_->Update();
	material_ = std::make_unique<ModelMaterial>(
		"FireVS.cso", 2,
		"FirePS.cso", 1
	);
	VECTOR pos = transform_->pos;
	material_->AddConstBufVS({ time_, pos.x, pos.y, pos.z });
	material_->AddConstBufVS({ RADIUS * 2.0f, 0.0f, 0.0f, 0.0f });
	material_->AddConstBufPS({ time_, 0.0f, 1.0f, 1.0f });

	material_->SetTextureBuf(3, ResourceManager::GetInstance().Load(ResourceManager::SRC::NOISE).handleId_);
	renderer_ = std::make_shared<ModelRenderer>(
		transform_->modelId, *material_
	);
	initRadian_ = initRad;
	num_ = num;
}

CrossLine::~CrossLine(void)
{
}

void CrossLine::Init(void)
{
}

void CrossLine::Update(void)
{
	time_ += SceneManager::GetInstance().GetDeltaTime();
	transform_->pos.x = centerPos_.x + (num_ * RADIUS + DISTANCE) * cosf(radian_ + initRadian_ );
	transform_->pos.y = centerPos_.y;
	transform_->pos.z = centerPos_.z + (num_ * RADIUS + DISTANCE) * sinf(radian_ + initRadian_ );
	transform_->Update();
	VECTOR pos = transform_->pos;
	material_->SetConstBufVS(0,{ time_, pos.x, pos.y, pos.z });
	material_->SetConstBufPS(0, { time_, 0.0f,0.0f, 0.0f });
}

void CrossLine::Draw(void)
{
	//MV1DrawModel(transform_->modelId);
	DrawTranslucentManager::GetInstance().Add(transform_, renderer_);
	//DrawSphere3D(transform_->pos, RADIUS, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), true);
}
