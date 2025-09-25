#include <DxLib.h>
#include "../EnemyBase.h"
#include "AttackBase.h"

AttackBase::AttackBase(EnemyBase& enemy) : enemy_(enemy)
{
	state_ = STATE::NONE;
	changeState_[STATE::NONE] = std::bind(&AttackBase::ChangeStateNone, this);
	changeState_[STATE::READY] = std::bind(&AttackBase::ChangeStateReady, this);
	changeState_[STATE::START] = std::bind(&AttackBase::ChangeStateStart, this);
	changeState_[STATE::UPDATE] = std::bind(&AttackBase::ChangeStateUpdate, this);
	changeState_[STATE::FINISH] = std::bind(&AttackBase::ChangeStateFinish, this);

}

AttackBase::~AttackBase()
{
}

void AttackBase::Init()
{
}

void AttackBase::Update()
{
}

void AttackBase::Draw()
{
}

void AttackBase::ChangeState(STATE state)
{
	state_ = state;
	changeState_[state]();
}

void AttackBase::ChangeStateNone(void)
{
}

void AttackBase::ChangeStateReady(void)
{
}

void AttackBase::ChangeStateStart(void)
{
}

void AttackBase::ChangeStateUpdate(void)
{
}

void AttackBase::ChangeStateFinish(void)
{
}
