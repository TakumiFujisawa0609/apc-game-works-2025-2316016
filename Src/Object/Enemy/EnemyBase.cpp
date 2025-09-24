#include "EnemyBase.h"
#include "../Common/Transform.h"
#include "../Common/Gravity.h"

EnemyBase::EnemyBase(Transform& target) : target_(target)
{
	transform_ = std::make_unique<Transform>();
	maxHP_ = 100.0f;
	hp_ = maxHP_;
}

EnemyBase::~EnemyBase(void)
{
}

void EnemyBase::Init(void)
{
}

void EnemyBase::Update(void)
{
	if (CheckHitKey(KEY_INPUT_Q))
	{
		hp_ -= 0.5f;
	}
}

void EnemyBase::Draw(void)
{
	float size = 50.0f;
	DrawSphere3D(transform_->pos, size, 16, 0,0, true);
}

void EnemyBase::Damage(float damage)
{
}
