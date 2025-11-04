#include "../../Utility/Utility.h"
#include "../Common/Transform.h"
#include "../Common/EffectController.h"
#include "../../Renderer/ModelMaterial.h"
#include "../../Renderer/ModelRenderer.h"
#include "Floor.h"
#include "ShockWave.h"
#include "Stage.h"

Stage::Stage(void)
{
	floor_ = std::make_unique<Floor>();
	for (int i = 0; i < WAVE_VERTEX_NUM; ++i)
	{
		shockWave_[i] = std::make_unique<ShockWave>(Utility::ONE_TRACK_DEG / WAVE_VERTEX_NUM * i);
	}
}

Stage::~Stage(void)
{
}

void Stage::Init(void)
{
}

void Stage::Update(void)
{
	floor_->Update();
	for (int i = 0; i < WAVE_VERTEX_NUM; ++i)
	{
		shockWave_[i]->Update();
	}
}

void Stage::Draw(void)
{
	floor_->Draw();
	for (int i = 0; i < WAVE_VERTEX_NUM; ++i)
	{
		shockWave_[i]->Draw();
	}
}
