#include "../../Utility/Utility.h"
#include "../Common/Transform.h"
#include "../Common/EffectController.h"
#include "../../Renderer/ModelMaterial.h"
#include "../../Renderer/ModelRenderer.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/DrawTranslucentManager.h"
#include "Stage.h"
#include "ShockWave.h"

ShockWave::ShockWave(float deg)
{
	time_ = 0.0f;
	transform_ = std::make_shared<Transform>();
	transform_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::SHOCK_WAVE1));
	float rad = Utility::Deg2RadF(deg);
	transform_->pos = { sinf(rad) * Stage::RADIUS,-10.0f,cosf(rad) * Stage::RADIUS};
	transform_->quaRot = Quaternion(VGet(0.0f, rad, 0.0f));
	transform_->quaRotLocal = Quaternion(VGet(0.0f, Utility::Deg2RadF(90.0f), 0.0f));
	transform_->scl = SIZE;
	transform_->Update();

	material_ = std::make_unique<ModelMaterial>(
		"ShockWaveVS.cso", 1,
		"ShockWavePS.cso", 1
	);
	material_->AddConstBufVS({ time_, WAVE_SCALE, 1.0f, 1.0f });
	material_->AddConstBufPS({ time_, WAVE_SCALE, 1.0f, 1.0f });

	material_->SetTextureBuf(3, ResourceManager::GetInstance().Load(ResourceManager::SRC::NOISE).handleId_);
	renderer_ = std::make_shared<ModelRenderer>(
		transform_->modelId, *material_
	);
}

ShockWave::~ShockWave(void)
{
}

void ShockWave::Init(void)
{
}

void ShockWave::Update(void)
{
	time_ += SceneManager::GetInstance().GetDeltaTime();
	material_->SetConstBufVS(0, { time_, WAVE_SCALE, 1.0f, 1.0f });
	material_->SetConstBufPS(0, { time_, WAVE_SCALE, 1.0f, 1.0f });
}

void ShockWave::Draw(void)
{
	//Utility::DrawCircle3DXZ(VGet(0.0f, 0.0f, 0.0f), RADIUS, VERTEX_NUM, 0xff00ff, false);
	//MV1DrawModel(transform_->modelId);
	//renderer_->Draw();
	DrawTranslucentManager::GetInstance().Add(transform_, renderer_);
}
