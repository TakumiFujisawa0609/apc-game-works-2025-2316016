#include "../../Common/Transform.h"
#include "../../Common/Gravity.h"
#include "../../Utility/Utility.h"
#include "../../Manager/SceneManager.h"
#include "Wave.h"
#include "../EnemyBase.h"
#include "JumpAttack.h"

JumpAttack::JumpAttack(EnemyBase& enemy) : AttackBase(enemy)
{
	range_ = RANGE::SHORT;
	geo_ = GEOMETORY::CIRCUMFERENCE;
}

JumpAttack::~JumpAttack()
{
}

void JumpAttack::Init(void)
{
}

void JumpAttack::Update(void)
{
	updateState_();
}

void JumpAttack::Draw(void)
{
	for (auto& wave : wave_)
	{
		if (wave->IsEnd())continue;
		wave->Draw();
	}
}

void JumpAttack::GetWaveState(float& radius, VECTOR& centerPos,int waveNum)
{
	radius = wave_[waveNum]->GetRadius();
	centerPos = wave_[waveNum]->GetPos();
}

void JumpAttack::Jump(void)
{
}

void JumpAttack::ChangeStateNone(void)
{
	AttackBase::ChangeStateNone();
}

void JumpAttack::ChangeStateReady(void)
{
	//ジャンプさせる
	AttackBase::ChangeStateReady();
	enemy_.GetGravity().ChengeState(Gravity::STATE::JUMP);
	enemy_.GetGravity().SetDir(Utility::DIR_D);
	enemy_.GetGravity().SetInitPower(JUMP_POW);
}

void JumpAttack::ChangeStateStart(void)
{
	//ウェーブの作成
	AttackBase::ChangeStateStart();
	std::unique_ptr<Wave> slow = std::make_unique<Wave>(enemy_.GetTransform().pos, Wave::SPEED_TYPE::SLOW, GetColor(255, 0, 0));
	std::unique_ptr<Wave> midium = std::make_unique<Wave>(enemy_.GetTransform().pos, Wave::SPEED_TYPE::MIDIUM, GetColor(255, 0, 0));
	std::unique_ptr<Wave> fast = std::make_unique<Wave>(enemy_.GetTransform().pos, Wave::SPEED_TYPE::FAST, GetColor(255, 0, 0));
	wave_.push_back(std::move(slow));
	wave_.push_back(std::move(midium));
	wave_.push_back(std::move(fast));
}

void JumpAttack::ChangeStateUpdate(void)
{
	AttackBase::ChangeStateUpdate();
}

void JumpAttack::ChangeStateFinish(void)
{
	AttackBase::ChangeStateFinish();
	deleyTime_ = COOL_DOWN;
}

void JumpAttack::UpdateStateNone(void)
{
}

void JumpAttack::UpdateStateReady(void)
{
	if (enemy_.GetGravity().GetState() != Gravity::STATE::JUMP)
	{
		ChangeState(STATE::START);
	}
}

void JumpAttack::UpdateStateStart(void)
{
	ChangeState(STATE::UPDATE);
}

void JumpAttack::UpdateStateUpdate(void)
{

	for (auto& wave : wave_)
	{
		wave->Update();
	}
	for (auto& wave : wave_)
	{
		if (!wave->IsEnd())
		{
			return;
		}
	}
	//std::erase_if(wave_, [](auto& wave) {return wave->IsShot();});
	Utility::EraseVectorAllay(wave_);
	ChangeState(STATE::FINISH);

}

void JumpAttack::UpdateStateFinish(void)
{
	deleyTime_ -= SceneManager::GetInstance().GetDeltaTime();
	if (deleyTime_ < 0.0f)
	{
		ChangeState(STATE::NONE);
	}
}
