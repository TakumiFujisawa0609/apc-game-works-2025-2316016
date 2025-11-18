#include "../../Utility/Utility.h"
#include "Stage.h"

Stage::Stage(PointLight& light)
{
	floor_ = std::make_unique<Floor>(light);
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
