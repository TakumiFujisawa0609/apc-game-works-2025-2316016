#include "../../../Common/Transform.h"
#include "../../../Common/Gravity.h"
#include "../../../Common/AnimationController.h"
#include "../../Utility/Utility.h"
#include "../../Manager/SceneManager.h"
#include "../SubObject/Wave.h"
#include "../../EnemyBase.h"
#include "JumpAttack.h"

JumpAttack::JumpAttack(EnemyBase& enemy) : AttackBase(enemy)
{
	range_ = RANGE::SHORT;
	geo_ = GEOMETORY::CIRCUMFERENCE;
	myType_ = EnemyBase::ATTACK_TYPE::JUMP;
	isDown_ = false;
}

JumpAttack::~JumpAttack(void)
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

void JumpAttack::ChangeStateNone(void)
{
	AttackBase::ChangeStateNone();
	isDown_ = false;
}

void JumpAttack::ChangeStateReady(void)
{
	//ジャンプさせる
	AttackBase::ChangeStateReady();
	enemy_.GetGravity().ChengeState(Gravity::STATE::JUMP);
	enemy_.GetGravity().SetDir(Utility::DIR_D);
	enemy_.GetGravity().SetInitPower(JUMP_POW);
	auto& animCtr = enemy_.GetAnimController();
	
	animCtr.Play((int)EnemyBase::ANIM_TYPE_DRAGON::TAKE_OFF, true, 0.0f, -1.0f);
}

void JumpAttack::ChangeStateStart(void)
{
	//ウェーブの作成
	AttackBase::ChangeStateStart();
	std::unique_ptr<Wave> slow = std::make_unique<Wave>(enemy_.GetTransform().pos, Wave::SPEED_TYPE::SLOW,Utility::RED);
	std::unique_ptr<Wave> midium = std::make_unique<Wave>(enemy_.GetTransform().pos, Wave::SPEED_TYPE::MIDIUM, Utility::RED);
	std::unique_ptr<Wave> fast = std::make_unique<Wave>(enemy_.GetTransform().pos, Wave::SPEED_TYPE::FAST, Utility::RED);
	wave_.push_back(std::move(slow));
	wave_.push_back(std::move(midium));
	wave_.push_back(std::move(fast));
	for (int i = 0; i < RANDOM_WAVE_NUM; i++)
	{
		std::unique_ptr<Wave> random = std::make_unique<Wave>(enemy_.GetTransform().pos, Wave::SPEED_TYPE::RANDOM, Utility::RED);
		wave_.push_back(std::move(random));
	}
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
	auto& animCtr = enemy_.GetAnimController();
	auto& gravity = enemy_.GetGravity();
	if (gravity.GetPower() > 0.0f && isDown_ == false)
	{
		isDown_ = true;
		animCtr.Play((int)EnemyBase::ANIM_TYPE_DRAGON::LAND, true, 0.0f, -1.0f,0.01f ,false, true, false);
	}
	if (enemy_.GetGravity().GetState() != Gravity::STATE::JUMP)
	{
		ChangeState(STATE::START);
		animCtr.Play((int)EnemyBase::ANIM_TYPE_DRAGON::SCREAM);
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
	wave_.clear();
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
