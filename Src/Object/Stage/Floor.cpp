#include "../../Utility/Utility.h"
#include "../Common/Transform.h"
#include "../Common/EffectController.h"
#include "../../Renderer/ModelMaterial.h"
#include "../../Renderer/ModelRenderer.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "Floor.h"

Floor::Floor(void)
{
	transform_ = std::make_shared<Transform>();
	transform_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::FLOOR));
	transform_->pos = { 0.0f,-10.0f,0.0f };
	transform_->scl = { SIZE,SIZE,SIZE };
	transform_->Update();

	material_ = std::make_unique<ModelMaterial>(
		"FloorVS.cso", 1,
		"FloorPS.cso", 0
	);
	material_->AddConstBufVS({ TEXTURE_SCALE, 0.0f, 1.0f, 1.0f });
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
}

void Floor::Draw(void)
{
	renderer_->Draw();
}

