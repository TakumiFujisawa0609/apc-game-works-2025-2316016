#include <DxLib.h>
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Renderer/ModelMaterial.h"
#include "../../Renderer/ModelRenderer.h"
#include "../Common/Transform.h"
#include "../Common/EffectController.h"
#include "SkyDome.h"

SkyDome::SkyDome(void)
{
	transform_ = std::make_unique<Transform>();
	transform_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::SKY_DOME));
	transform_->pos = { 0.0f,0.0f,0.0f };
	transform_->scl = { SCALE,SCALE,SCALE };
	transform_->Update();

	material_ = std::make_unique<ModelMaterial>(
		"SkyDomeVS.cso", 0,
		"SkyDomePS.cso", 1
	);
	material_->AddConstBufPS(DEFAULT_COLOR);
	renderer_ = std::make_shared<ModelRenderer>(transform_->modelId, *material_	);
}

SkyDome::~SkyDome(void)
{
}

void SkyDome::Init(void)
{

	//material_->AddConstBufVS({ TEXTURE_SCALE, 0.0f, 1.0f, 1.0f });
	//material_->SetTextureBuf(3, ResourceManager::GetInstance().Load(ResourceManager::SRC::NOISE).handleId_);

	//material_ = std::make_unique<ModelMaterial>(
	//	"FloorVS.cso", 1,
	//	"FloorPS.cso", 0
	//);
	//material_->AddConstBufVS({ TEXTURE_SCALE, 0.0f, 1.0f, 1.0f });
	//material_->SetTextureBuf(3, ResourceManager::GetInstance().Load(ResourceManager::SRC::NOISE).handleId_);
	//renderer_ = std::make_unique<ModelRenderer>(
	//	transform_->modelId, *material_
	//);
}

void SkyDome::Update(void)
{
	time_ += SceneManager::GetInstance().GetDeltaTime();
	transform_->quaRot = Quaternion(VECTOR(0.0f, Utility::Deg2RadF(ROTAT_DEG_SEC * time_), 0.0f));
	transform_->Update();
}

void SkyDome::Draw(void)
{
	//MV1DrawModel(transform_->modelId);
	renderer_->Draw();
}

void SkyDome::SetColor(FLOAT4 color)
{
	material_->SetConstBufPS(0, color);
}
