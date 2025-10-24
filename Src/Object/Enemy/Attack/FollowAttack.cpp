#include "../../Utility/Utility.h"
#include "../../Common/Transform.h"
#include "../../Manager/SceneManager.h"
#include "../EnemyBase.h"
#include "FollowShot.h"
#include "FollowAttack.h"

FollowAttack::FollowAttack(EnemyBase& enemy) : AttackBase(enemy)
{
	range_ = RANGE::LONG;
	geo_ = GEOMETORY::SPHERE;
	myType_ = EnemyBase::ATTACK_TYPE::FOLLOW;
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

Transform& FollowAttack::GetShotTransform(int shotNum)
{
	return shots_[shotNum]->GetTransform();
}

void FollowAttack::ChangeStateNone(void)
{
	AttackBase::ChangeStateNone();
}

void FollowAttack::ChangeStateReady(void)
{
	AttackBase::ChangeStateReady();
}

void FollowAttack::ChangeStateStart(void)
{
	AttackBase::ChangeStateStart();
	std::unique_ptr<FollowShot> slow = std::make_unique<FollowShot>(*target_,FollowShot::SPEED_TYPE::SLOW,enemy_.GetTransform().pos);
	std::unique_ptr<FollowShot> midium = std::make_unique<FollowShot>(*target_, FollowShot::SPEED_TYPE::MIDIUM, enemy_.GetTransform().pos);
	std::unique_ptr<FollowShot> fast = std::make_unique<FollowShot>(*target_, FollowShot::SPEED_TYPE::FAST, enemy_.GetTransform().pos);
	shots_.push_back(std::move(slow));
	shots_.push_back(std::move(midium));
	shots_.push_back(std::move(fast));
	for (int i = 0; i < RANDOM_SHOT_NUM; i++)
	{
		std::unique_ptr<FollowShot> random = std::make_unique<FollowShot>(*target_, FollowShot::SPEED_TYPE::RANDOM, enemy_.GetTransform().pos);
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
	ChangeState(STATE::START);
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
