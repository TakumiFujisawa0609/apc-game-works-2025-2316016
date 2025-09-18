#include <DxLib.h>
#include "../../Manager/ResourceManager.h"
#include "SkyDome.h"

SkyDome::SkyDome(void)
{
}

SkyDome::~SkyDome(void)
{
}

void SkyDome::Init(void)
{
	modelHandle_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::SKY_DOME).handleId_;
	MV1SetPosition(modelHandle_, VGet(0.0f, 0.0f, 0.0f));
	MV1SetScale(modelHandle_, VGet(SCALE, SCALE, SCALE));
}

void SkyDome::Update(void)
{
}

void SkyDome::Draw(void)
{
	MV1DrawModel(modelHandle_);
}
