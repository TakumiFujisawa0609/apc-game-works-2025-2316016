#include "../../Manager/SceneManager.h"
#include "FallDownAttack.h"

FallDownAttack::FallDownAttack(EnemyBase& enemy) : AttackBase(enemy)
{
	range_ = RANGE::ALL;
	geo_ = GEOMETORY::SPHERE;
}

FallDownAttack::~FallDownAttack(void)
{
}

void FallDownAttack::Init(void)
{
}

void FallDownAttack::Update(void)
{
}

void FallDownAttack::Draw(void)
{
}

void FallDownAttack::ChangeStateNone(void)
{
	AttackBase::ChangeStateNone();
}

void FallDownAttack::ChangeStateReady(void)
{
	AttackBase::ChangeStateReady();
}

void FallDownAttack::ChangeStateStart(void)
{
	AttackBase::ChangeStateStart();
}

void FallDownAttack::ChangeStateUpdate(void)
{
	AttackBase::ChangeStateUpdate();
}

void FallDownAttack::ChangeStateFinish(void)
{
	deleyTime_ = COOL_DOWN;
	AttackBase::ChangeStateFinish();
}

void FallDownAttack::UpdateStateNone(void)
{
}

void FallDownAttack::UpdateStateReady(void)
{
}

void FallDownAttack::UpdateStateStart(void)
{
}

void FallDownAttack::UpdateStateUpdate(void)
{
}

void FallDownAttack::UpdateStateFinish(void)
{
	deleyTime_ -= SceneManager::GetInstance().GetDeltaTime();
	if (deleyTime_ < 0.0f)
	{
		ChangeState(STATE::NONE);
	}
}
