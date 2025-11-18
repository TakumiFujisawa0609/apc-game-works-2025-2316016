#include "../../Utility/Utility.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../Light/PointLight.h"
#include "Floor.h"

Floor::Floor(PointLight& light) : light_(light)
{
	VECTOR lightPos = light_.GetLightPos();
	float lightRadius = light_.GetRadius();
	transform_ = std::make_shared<Transform>();
	transform_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::FLOOR));
	transform_->pos = { 0.0f,-10.0f,0.0f };
	transform_->scl = { SIZE,SIZE,SIZE };
	transform_->Update();

	material_ = std::make_unique<ModelMaterial>(
		"FloorVS.cso", 1,
		"FloorPS.cso", 2
	);
	material_->AddConstBufVS({ TEXTURE_SCALE, 0.0f, 1.0f, 1.0f });
	material_->AddConstBufPS(Utility::COLOR_F2FLOAT4(LIGHT_COLOR));
	material_->AddConstBufPS({ lightPos.x, lightPos.y, lightPos.z, lightRadius });
	material_->SetTextureBuf(3, ResourceManager::GetInstance().Load(ResourceManager::SRC::NOISE).handleId_);
	renderer_ = std::make_shared<ModelRenderer>(
		transform_->modelId, *material_
	);
}

Floor::~Floor(void)
{
}

void Floor::Init(void)
{
}

void Floor::Update(void)
{
	VECTOR lightPos = light_.GetLightPos();
	float lightRadius = light_.GetRadius();
	material_->SetConstBufPS(1,{ lightPos.x, lightPos.y, lightPos.z, lightRadius });
}

void Floor::Draw(void)
{
	renderer_->Draw();
}

