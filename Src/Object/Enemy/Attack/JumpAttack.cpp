#include "../../Common/Transform.h"
#include "../../Common/Gravity.h"
#include "Wave.h"
#include "../EnemyBase.h"
#include "JumpAttack.h"

JumpAttack::JumpAttack(EnemyBase& enemy) : AttackBase(enemy)
{
	range_ = RANGE::SHORT;
}

JumpAttack::~JumpAttack()
{
}

void JumpAttack::Init(void)
{
}

void JumpAttack::Update(void)
{
	for (auto& wave : wave_)
	{
		wave->Update();
	}
}

void JumpAttack::Draw(void)
{
	for (auto& wave : wave_)
	{
		wave->Draw();
	}
}

void JumpAttack::Jump(void)
{
}

void JumpAttack::ChangeStateNone(void)
{
}

void JumpAttack::ChangeStateReady(void)
{
	enemy_.GetGravity().ChengeState(Gravity::STATE::JUMP);
	enemy_.GetGravity().SetInitPower(JUMP_POW);
}

void JumpAttack::ChangeStateStart(void)
{
	std::unique_ptr<Wave> slow = std::make_unique<Wave>(enemy_.GetTransform().pos, Wave::SPEED_TYPE::SLOW, GetColor(255, 0, 0));
	std::unique_ptr<Wave> midium = std::make_unique<Wave>(enemy_.GetTransform().pos, Wave::SPEED_TYPE::MIDIUM, GetColor(255, 0, 0));
	std::unique_ptr<Wave> fast = std::make_unique<Wave>(enemy_.GetTransform().pos, Wave::SPEED_TYPE::FAST, GetColor(255, 0, 0));
	wave_.push_back(std::move(slow));
	wave_.push_back(std::move(midium));
	wave_.push_back(std::move(fast));
}

void JumpAttack::ChangeStateUpdate(void)
{
}

void JumpAttack::ChangeStateFinish(void)
{
}
