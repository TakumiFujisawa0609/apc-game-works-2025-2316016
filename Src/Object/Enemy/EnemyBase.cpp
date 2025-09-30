#include <cmath>
#include <memory>
#include "../../Utility/Utility.h"
#include "../../Manager/ResourceManager.h"
#include "../Player/PlayerBase.h"
#include "../Common/Transform.h"
#include "../Common/Gravity.h"
#include "Attack/AttackBase.h"
#include "Attack/JumpAttack.h"
#include "EnemyBase.h"

EnemyBase::EnemyBase(Transform& target) : target_(target)
{
	transform_ = std::make_unique<Transform>();
	transform_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ENEMY));
	transform_->scl = { MODEL_SIZE,MODEL_SIZE,MODEL_SIZE };
	transform_->Update();
	gravity_ = std::make_unique<Gravity>();
	gravity_->Init();
	gravity_->SetDir(Utility::DIR_D);
	maxHP_ = 100.0f;
	hp_ = maxHP_;
	AplayChangeStateFunc();
	AddAttack(ATTACK_TYPE::JUMP);
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
	for (auto& attack : attackList_)
	{
		attack->Update();
	}
	updateState_();
	gravity_->Update();
	AplayGravity();
	MoveLimit();
	transform_->Update();
}

void EnemyBase::Draw(void)
{
	//float size = 50.0f;
	MV1DrawModel(transform_->modelId);
	for (auto& attack : attackList_)
	{
		attack->Draw();
	}
}

void EnemyBase::Damage(float damage)
{
	hp_ -= damage;
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
	if (prePos.y != pos.y)
	{
		gravity_->ChengeState(Gravity::STATE::NONE);
	}
}

void EnemyBase::AplayGravity(void)
{
	transform_->pos = VAdd(transform_->pos, VScale(gravity_->GetDir(), gravity_->GetPower()));
}

void EnemyBase::ChangeStateIdle(void)
{
	updateState_ = std::bind(&EnemyBase::UpdateIdle, this);
}

void EnemyBase::ChangeStateAttack(void)
{
	updateState_ = std::bind(&EnemyBase::UpdateAttack, this);
}

void EnemyBase::ChangeStateDead(void)
{
	updateState_ = std::bind(&EnemyBase::UpdateDead, this);
}

void EnemyBase::UpdateIdle(void)
{
	float dis = Utility::MagnitudeF(VSub(transform_->pos, target_.pos));
	//優先範囲を求める
	AttackBase::RANGE priorityRange;	
	if (dis < AttackBase::SHORT_RANGE)
	{
		priorityRange = AttackBase::RANGE::SHORT;
	}
	else if (dis < AttackBase::MIDDLE_RANGE)
	{
		priorityRange = AttackBase::RANGE::MIDDLE;
	}
	else
	{
		priorityRange = AttackBase::RANGE::LONG;
	}
	//優先度が高いものを格納する
	std::vector<std::unique_ptr<AttackBase>> priorityAttackList;
	for (auto& attack : attackList_)
	{
		//優先距離内でクールタイムが明けているものを移動
		auto range = attack->GetRange();
		if ((range == priorityRange || range == AttackBase::RANGE::ALL) && attack->GetState() == AttackBase::STATE::NONE)
		{
			priorityAttackList.push_back(std::move(attack));
		}
	}
	int size = static_cast<int>(priorityAttackList.size());
	if (size == 0)
	{
		//優先距離外でクールタイムが開けているモノを移動
		for (auto& attack : attackList_)
		{
			if (attack->GetState() != AttackBase::STATE::NONE)continue;
			priorityAttackList.push_back(std::move(attack));
		}
		if(static_cast<int>(priorityAttackList.size()) == 0)return;
	}
	//nullptrの場所を開放する
	//std::erase_if(attackList_, [](auto& attack) {return attack == nullptr;});
	Utility::EraseVectorAllay(attackList_);
	//優先度が同じものをランダムで選択
	int i = GetRand(size - 1);
	priorityAttackList[i]->ChangeState(AttackBase::STATE::READY);
	for (auto& attack : priorityAttackList)
	{
		attackList_.push_back(std::move(attack));
	}
	ChangeState(STATE::ATTACK);
}

void EnemyBase::UpdateAttack(void)
{
	for (auto& attack : attackList_)
	{
		//稼働中のものがあるかを調べる
		if (!(attack->GetState() == AttackBase::STATE::NONE || attack->GetState() == AttackBase::STATE::FINISH))
		{
			return;
		}
	}
	//稼働中のものがなければIDLEに戻す
	ChangeState(STATE::IDLE);
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
	attackList_.push_back(std::move(attack));
}

void EnemyBase::AplayChangeStateFunc(void)
{
	changeState_[(STATE::IDLE)] = std::bind(&EnemyBase::ChangeStateIdle, this);
	changeState_[(STATE::ATTACK)] = std::bind(&EnemyBase::ChangeStateAttack, this);
	changeState_[(STATE::DEAD)] = std::bind(&EnemyBase::ChangeStateDead, this);
}
