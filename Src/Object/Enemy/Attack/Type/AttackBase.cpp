#include <DxLib.h>
#include "../../Renderer/ModelMaterial.h"
#include "../../Renderer/ModelRenderer.h"
#include "../../../Common/EffectController.h"
#include "../../../Common/Transform.h"
#include "AttackBase.h"

AttackBase::AttackBase(EnemyBase& enemy) : enemy_(enemy)
{
	//チェンジステートをステートごとに格納
	changeState_[STATE::NONE] = std::bind(&AttackBase::ChangeStateNone, this);
	changeState_[STATE::READY] = std::bind(&AttackBase::ChangeStateReady, this);
	changeState_[STATE::START] = std::bind(&AttackBase::ChangeStateStart, this);
	changeState_[STATE::UPDATE] = std::bind(&AttackBase::ChangeStateUpdate, this);
	changeState_[STATE::FINISH] = std::bind(&AttackBase::ChangeStateFinish, this);
	ChangeState(STATE::NONE);

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

int AttackBase::GetSubObjectNum()
{
	return -1;
}

void AttackBase::ChangeState(STATE state)
{
	state_ = state;
	changeState_[state]();
}

void AttackBase::ChangeStateNone(void)
{
	updateState_ = std::bind(&AttackBase::UpdateStateNone, this);
}

void AttackBase::ChangeStateReady(void)
{
	updateState_ = std::bind(&AttackBase::UpdateStateReady, this);
}

void AttackBase::ChangeStateStart(void)
{
	updateState_ = std::bind(&AttackBase::UpdateStateStart, this);
}

void AttackBase::ChangeStateUpdate(void)
{
	updateState_ = std::bind(&AttackBase::UpdateStateUpdate, this);
}

void AttackBase::ChangeStateFinish(void)
{
	updateState_ = std::bind(&AttackBase::UpdateStateFinish, this);
}

void AttackBase::UpdateStateNone(void)
{
}

void AttackBase::UpdateStateReady(void)
{
}

void AttackBase::UpdateStateStart(void)
{
}

void AttackBase::UpdateStateUpdate(void)
{
}

void AttackBase::UpdateStateFinish(void)
{
}
