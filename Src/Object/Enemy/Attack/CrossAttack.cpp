#include "../../Utility/Utility.h"
#include "../../Manager/SceneManager.h"
#include "../EnemyBase.h"
#include "CrossLine.h"
#include "CrossAttack.h"

CrossAttack::CrossAttack(EnemyBase& enemy) : AttackBase(enemy)
{
	range_ = RANGE::ALL;
	geo_ = GEOMETORY::SPHERE;
	time_ = 0.0f;
	radian_ = 0.0f;
}
CrossAttack::~CrossAttack(void)
{
}

void CrossAttack::Init(void)
{
}

void CrossAttack::Update(void)
{
	updateState_();
	for (auto& line : crossLines_)
	{
		line->Update();
	}
}

void CrossAttack::Draw(void)
{
	for (auto& line : crossLines_)
	{
		line->Draw();
	}
}

Transform& CrossAttack::GetLineTransform(int lineNum)
{
	 return crossLines_[lineNum]->GetTransform();
}

void CrossAttack::ChangeStateNone(void)
{
	AttackBase::ChangeStateNone();
}

void CrossAttack::ChangeStateReady(void)
{
	AttackBase::ChangeStateReady();
}

void CrossAttack::ChangeStateStart(void)
{
	AttackBase::ChangeStateStart();
}

void CrossAttack::ChangeStateUpdate(void)
{
	AttackBase::ChangeStateUpdate();
}

void CrossAttack::ChangeStateFinish(void)
{
	deleyTime_ = COOL_DOWN;
	AttackBase::ChangeStateFinish();
}

void CrossAttack::UpdateStateNone(void)
{
}

void CrossAttack::UpdateStateReady(void)
{
	ChangeState(STATE::START);
}

void CrossAttack::UpdateStateStart(void)
{
	for (int i = 0; i < LINE_NUM; i++)
	{
		for (int j = 0; j < LINE_POINT_NUM; j++)
		{
			std::unique_ptr<CrossLine> line;
			line = std::make_unique<CrossLine>(enemy_.GetTransform().pos, radian_, Utility::Deg2RadF(i * 90.0f), j);
			crossLines_.push_back(std::move(line));
		}
	}
	ChangeState(STATE::UPDATE);
}

void CrossAttack::UpdateStateUpdate(void)
{
	time_ += SceneManager::GetInstance().GetDeltaTime();
	radian_ = Utility::Deg2RadF(time_ * SECOND_TO_DEGREE);
	if (time_ > TIME)
	{
		crossLines_.clear();
		ChangeState(STATE::FINISH);
	}
}

void CrossAttack::UpdateStateFinish(void)
{
	deleyTime_ -= SceneManager::GetInstance().GetDeltaTime();
	if (deleyTime_ < 0.0f)
	{
		time_ = 0.0f;
		ChangeState(STATE::NONE);
	}
}
