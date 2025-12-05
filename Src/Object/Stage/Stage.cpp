#include "../../Utility/Utility.h"
#include "Stage.h"

Stage::Stage(PointLight& light)
{
	floor_ = std::make_unique<Floor>(light);
	shockWave_ = std::make_unique<ShockWave>();
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
	shockWave_->Update();
}

void Stage::Draw(void)
{
	floor_->Draw();
	shockWave_->Draw();
}
