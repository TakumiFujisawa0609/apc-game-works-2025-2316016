#include "../../../../Manager/ResourceManager.h"
#include "../../../Common/Transform.h"
#include "CrossLine.h"

CrossLine::CrossLine(VECTOR centerPos, float& rad, float initRad, int num) : centerPos_(centerPos), radian_(rad)
{
	transform_ = std::make_unique<Transform>();
	transform_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ENEMY));
	transform_->scl = VGet(2.0f, 2.0f, 2.0f);
	transform_->Update();
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
	transform_->pos.x = centerPos_.x + num_ * RADIUS * cosf(radian_ + initRadian_ );
	transform_->pos.y = centerPos_.y;
	transform_->pos.z = centerPos_.z + num_ * RADIUS * sinf(radian_ + initRadian_ );
	transform_->Update();
}

void CrossLine::Draw(void)
{
	MV1DrawModel(transform_->modelId);
	DrawSphere3D(transform_->pos, RADIUS, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), true);
}
