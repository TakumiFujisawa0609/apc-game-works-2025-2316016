#include <cmath>
#include <memory>
#include "../Player/PlayerBase.h"
#include "../Common/Transform.h"
#include "../Common/Gravity.h"
#include "Attack/AttackBase.h"
#include "Attack/JumpAttack.h"
#include "EnemyBase.h"

EnemyBase::EnemyBase(Transform& target) : target_(target)
{
	transform_ = std::make_unique<Transform>();
	gravity_ = std::make_unique<Gravity>();
	gravity_->Init();
	maxHP_ = 100.0f;
	hp_ = maxHP_;
	AplayChangeStateFunc();
	ChangeState(STATE::IDLE);
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
	gravity_->Update();
	
	MoveLimit();
}

void EnemyBase::Draw(void)
{
	float size = 50.0f;
	DrawSphere3D(transform_->pos, size, 16, 0,0, true);
}

void EnemyBase::Damage(float damage)
{
}
void EnemyBase::ChangeState(STATE state)
{
	state_ = state;
	changeState_[state]();
}
void EnemyBase::MoveLimit(void)
{
	auto& pos = transform_->pos;
	VECTOR prePos = pos;
	pos.x = std::max(std::min(PlayerBase::MOVE_LIMIT_MAX.x, pos.x), PlayerBase::MOVE_LIMIT_MIN.x);
	pos.y = std::max(std::min(PlayerBase::MOVE_LIMIT_MAX.y, pos.y), PlayerBase::MOVE_LIMIT_MIN.y);
	pos.z = std::max(std::min(PlayerBase::MOVE_LIMIT_MAX.z, pos.z), PlayerBase::MOVE_LIMIT_MIN.z);

}

void EnemyBase::ChangeStateIdle(void)
{
}

void EnemyBase::ChangeStateAttack(void)
{
}

void EnemyBase::ChangeStateDead(void)
{
}

void EnemyBase::UpdateIdle(void)
{
}

void EnemyBase::UpdateAttack(void)
{
}

void EnemyBase::UpdateDead(void)
{
}

void EnemyBase::AddAttack(ATTACK_TYPE type)
{
	std::unique_ptr<AttackBase> attack;
	switch (type)
	{
	case EnemyBase::ATTACK_TYPE::JUMP:
		attack = std::make_unique<JumpAttack>(*this);
		break;
	case EnemyBase::ATTACK_TYPE::MAX:
		break;
	default:
		break;
	}
}

void EnemyBase::AplayChangeStateFunc(void)
{
	changeState_[(STATE::IDLE)] = std::bind(&EnemyBase::ChangeStateIdle, this);
	changeState_[(STATE::ATTACK)] = std::bind(&EnemyBase::ChangeStateAttack, this);
	changeState_[(STATE::DEAD)] = std::bind(&EnemyBase::ChangeStateDead, this);
}
