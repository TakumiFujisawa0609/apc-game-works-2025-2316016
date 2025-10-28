#include "../../Utility/Utility.h"
#include "../../Manager/SceneManager.h"
#include "../../../Common/Gravity.h"
#include "../../EnemyBase.h"
#include "../SubObject/Wave.h"
#include "JumpAttackConstant.h"

JumpAttackConstant::JumpAttackConstant(EnemyBase& enemy) : AttackBase(enemy)
{
	range_ = RANGE::MIDDLE;
	geo_ = GEOMETORY::CIRCUMFERENCE;
	time_ = 0.0f;
	intervalTime_ = 0.0f;
	myType_ = EnemyBase::ATTACK_TYPE::JUMP_CONSTANT;
}

JumpAttackConstant::~JumpAttackConstant(void)
{
}

void JumpAttackConstant::Init(void)
{
}

void JumpAttackConstant::Update(void)
{
	auto deltaTime = SceneManager::GetInstance().GetDeltaTime();
	time_ -= deltaTime;
	intervalTime_ -= deltaTime;
	updateState_();
}

void JumpAttackConstant::Draw(void)
{
	for (auto& wave : wave_)
	{
		if (wave->IsEnd())continue;
		wave->Draw();
	}
}

void JumpAttackConstant::GetWaveState(float& radius, VECTOR& centerPos, int waveNum)
{
	radius = wave_[waveNum]->GetRadius();
	centerPos = wave_[waveNum]->GetPos();
}


void JumpAttackConstant::ChangeStateNone(void)
{
	AttackBase::ChangeStateNone();
}

void JumpAttackConstant::ChangeStateReady(void)
{
	//ジャンプさせる
	AttackBase::ChangeStateReady();
	enemy_.GetGravity().ChengeState(Gravity::STATE::JUMP);
	enemy_.GetGravity().SetDir(Utility::DIR_D);
	enemy_.GetGravity().SetInitPower(JUMP_POW);
}

void JumpAttackConstant::ChangeStateStart(void)
{
	//ウェーブの作成
	AttackBase::ChangeStateStart();
	std::unique_ptr<Wave> fast = std::make_unique<Wave>(enemy_.GetTransform().pos, Wave::SPEED_TYPE::FAST, Utility::RED);
	wave_.push_back(std::move(fast));
	time_ = TIME;
	intervalTime_ = INTERVAL_TIME;
}

void JumpAttackConstant::ChangeStateUpdate(void)
{
	AttackBase::ChangeStateUpdate();
}

void JumpAttackConstant::ChangeStateFinish(void)
{
	AttackBase::ChangeStateFinish();
	deleyTime_ = COOL_DOWN;
}

void JumpAttackConstant::UpdateStateNone(void)
{
}

void JumpAttackConstant::UpdateStateReady(void)
{
	if (enemy_.GetGravity().GetState() != Gravity::STATE::JUMP)
	{
		ChangeState(STATE::START);
	}
}

void JumpAttackConstant::UpdateStateStart(void)
{
	ChangeState(STATE::UPDATE);
}

void JumpAttackConstant::UpdateStateUpdate(void)
{
	for (auto& wave : wave_)
	{
		wave->Update();
	}
	for (auto& wave : wave_)
	{
		if (wave->IsEnd())
		{
			wave = nullptr;
		}
	}
	Utility::EraseVectorAllay(wave_);

	if (intervalTime_ <= 0.0f && time_ > 0.0f)
	{
		std::unique_ptr<Wave> wave = std::make_unique<Wave>(enemy_.GetTransform().pos, Wave::SPEED_TYPE::FAST, Utility::RED);
		wave_.push_back(std::move(wave));
		intervalTime_ = INTERVAL_TIME;
	}
	if (time_ <= 0.0f && !(GetSubObjectNum() > 0))
	{
		ChangeState(STATE::FINISH);
	}
}

void JumpAttackConstant::UpdateStateFinish(void)
{
	deleyTime_ -= SceneManager::GetInstance().GetDeltaTime();
	if (deleyTime_ < 0.0f)
	{
		ChangeState(STATE::NONE);
	}
}
