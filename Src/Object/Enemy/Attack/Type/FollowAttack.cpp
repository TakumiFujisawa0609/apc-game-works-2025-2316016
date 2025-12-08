#include "../../Utility/Utility.h"
#include "../../../Common/Transform.h"
#include "../../../Common/AnimationController.h"
#include "../../Manager/SceneManager.h"
#include "../../EnemyBase.h"
#include "../SubObject/FollowShot.h"
#include "FollowAttack.h"

FollowAttack::FollowAttack(EnemyAttackManager& parent) : AttackBase(parent)
{
	range_ = RANGE::LONG;
	geo_ = GEOMETORY::SPHERE;
	myType_ = EnemyAttackManager::ATTACK_TYPE::FOLLOW;
}

FollowAttack::~FollowAttack(void)
{
}

void FollowAttack::Init(void)
{
}

void FollowAttack::Update(void)
{
	updateState_();
}

void FollowAttack::Draw(void)
{
	for (auto& shot : shots_)
	{
		if (!shot->IsShot())continue;
		shot->Draw();
	}
}

void FollowAttack::ChangeStateNone(void)
{
	AttackBase::ChangeStateNone();
}

void FollowAttack::ChangeStateReady(void)
{
	parent_.GetAnimController().Play(parent_.GetAnimNumber(EnemyAttackManager::ATTACK_STATE::READY, myType_),false);
	AttackBase::ChangeStateReady();
}

void FollowAttack::ChangeStateStart(void)
{
	parent_.GetAnimController().Play(parent_.GetAnimNumber(EnemyAttackManager::ATTACK_STATE::PLAY, myType_));
	AttackBase::ChangeStateStart();
	std::unique_ptr<FollowShot> slow = std::make_unique<FollowShot>(target_,FollowShot::SPEED_TYPE::SLOW,parent_.GetTransform().lock()->pos);
	std::unique_ptr<FollowShot> midium = std::make_unique<FollowShot>(target_, FollowShot::SPEED_TYPE::MIDIUM, parent_.GetTransform().lock()->pos);
	std::unique_ptr<FollowShot> fast = std::make_unique<FollowShot>(target_, FollowShot::SPEED_TYPE::FAST, parent_.GetTransform().lock()->pos);
	shots_.push_back(std::move(slow));
	shots_.push_back(std::move(midium));
	shots_.push_back(std::move(fast));
	for (int i = 0; i < RANDOM_SHOT_NUM; i++)
	{
		std::unique_ptr<FollowShot> random = std::make_unique<FollowShot>(target_, FollowShot::SPEED_TYPE::RANDOM, parent_.GetTransform().lock()->pos);
		shots_.push_back(std::move(random));
	}
}

void FollowAttack::ChangeStateUpdate(void)
{
	AttackBase::ChangeStateUpdate();
}

void FollowAttack::ChangeStateFinish(void)
{
	AttackBase::ChangeStateFinish();
	deleyTime_ = COOL_DOWN;
}

void FollowAttack::UpdateStateNone(void)
{
}

void FollowAttack::UpdateStateReady(void)
{
	if (parent_.GetAnimController().IsEnd())
	{
		ChangeState(STATE::START);
	}
}

void FollowAttack::UpdateStateStart(void)
{
	ChangeState(STATE::UPDATE);
}

void FollowAttack::UpdateStateUpdate(void)
{
	bool isFinish = true;
	for (auto& shot : shots_)
	{
		if (shot->IsShot())
		{
			isFinish = false;
			shot->Update();
		}
		else 
		{
			shot = nullptr;
		}
	}
	Utility::EraseVectorAllay(shots_); 
	if (isFinish)
	{
		ChangeState(STATE::FINISH);
	}
}

void FollowAttack::UpdateStateFinish(void)
{
	deleyTime_ -= SceneManager::GetInstance().GetDeltaTime();
	if (deleyTime_ < 0.0f)
	{
		ChangeState(STATE::NONE);
	}
}
