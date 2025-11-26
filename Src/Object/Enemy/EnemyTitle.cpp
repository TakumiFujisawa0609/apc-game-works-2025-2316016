#include "Type/Dragon.h"
#include "EnemyTitle.h"

EnemyTitle::EnemyTitle(std::weak_ptr<Transform> trans):EnemyBase(trans)
{
}

EnemyTitle::~EnemyTitle(void)
{
}

void EnemyTitle::Init(void)
{
}

void EnemyTitle::Update(void)
{
	dragon_->Update();
}

void EnemyTitle::Draw(void)
{
	EnemyBase::Draw();
}

void EnemyTitle::UIDraw(void)
{
}

void EnemyTitle::SetPos(VECTOR pos)
{
	transform_->pos = pos;
	transform_->Update();
}

void EnemyTitle::SetRot(VECTOR rot)
{
	transform_->quaRot = Quaternion(rot);
	transform_->Update();
}

void EnemyTitle::SetScl(VECTOR scl)
{
	transform_->scl = scl;
	transform_->Update();
}
