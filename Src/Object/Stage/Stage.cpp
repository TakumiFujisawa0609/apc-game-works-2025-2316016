#include "../../Utility/Utility.h"
#include "../Common/Transform.h"
#include "../../Renderer/ModelMaterial.h"
#include "../../Renderer/ModelRenderer.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "Stage.h"

Stage::Stage(void)
{
	time_ = 0.0f;
	transform_ = std::make_unique<Transform>();
	transform_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::SHOCK_WAVE));
	transform_->pos = { 0.0f,-10.0f,0.0f };
	transform_->scl = { SIZE,SIZE,SIZE };
	transform_->Update();

	material_ = std::make_unique<ModelMaterial>(
		"ShockWaveVS.cso", 1,
		"ShockWavePS.cso", 1
	);
	material_->AddConstBufVS({ time_, WAVE_SCALE, 1.0f, 1.0f });
	material_->AddConstBufPS({ time_, WAVE_SCALE, 1.0f, 1.0f });

	renderer_ = std::make_unique<ModelRenderer>(
		transform_->modelId, *material_
	);
}

Stage::~Stage(void)
{
}

void Stage::Init(void)
{
}

void Stage::Update(void)
{
	time_ += SceneManager::GetInstance().GetDeltaTime();
	material_->SetConstBufVS(0, { time_, WAVE_SCALE, 1.0f, 1.0f });
	material_->SetConstBufPS(0, { time_, WAVE_SCALE, 1.0f, 1.0f });
}

void Stage::Draw(void)
{
	//Utility::DrawCircle3DXZ(VGet(0.0f, 0.0f, 0.0f), RADIUS, VERTEX_NUM, 0xff00ff, false);
	//MV1DrawModel(transform_->modelId);
	renderer_->Draw();
}

const int Stage::GetModelId(void)
{
	return transform_->modelId;
}
