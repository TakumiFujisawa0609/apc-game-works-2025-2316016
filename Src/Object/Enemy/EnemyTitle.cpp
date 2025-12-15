#include "../Common/AnimationController.h"
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
	auto& animCtrl = dragon_->GetAnimController();
	if (GetRand(ANIM_POP) == 0)
	{
		animCtrl.Play((int)Dragon::ANIM_TYPE::SCREAM, false);
	}
	if (animCtrl.IsEnd())
	{
		animCtrl.Play((int)Dragon::ANIM_TYPE::IDLE_1);
	}
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

void EnemyTitle::InitAddAttack(void)
{
}
