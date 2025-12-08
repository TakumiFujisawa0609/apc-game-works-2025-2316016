#include "../../Utility/Utility.h"
#include "../Common/Transform.h"
#include "Attack/Type/AttackBase.h"
#include "Attack/Type/JumpAttack.h"
#include "Attack/Type/JumpAttackConstant.h"
#include "Attack/Type/ThunderAroundAttack.h"
#include "Attack/Type/CrossAttack.h"
#include "Attack/Type/FollowAttack.h"
#include "Attack/Type/FallDownAttack.h"
#include "Attack/Type/WaterSpritAttack.h"
#include "Type/Dragon.h"
#include "EnemyBase.h"
#include "EnemyAttackManager.h"


EnemyAttackManager::EnemyAttackManager(EnemyBase& enemy): enemy_(enemy)
{
}

EnemyAttackManager::~EnemyAttackManager(void)
{
}

void EnemyAttackManager::Init(void)
{

}

void EnemyAttackManager::Update(void)
{
	for (auto& attack : attackList_)
	{
		attack->Update();
	}
}

void EnemyAttackManager::Draw(void)
{
	for (auto& attack : attackList_)
	{
		attack->Draw();
	}
}

void EnemyAttackManager::AddAttack(ATTACK_TYPE type)
{
	std::unique_ptr<AttackBase> attack;
	switch (type)
	{
	case EnemyAttackManager::ATTACK_TYPE::JUMP:
		attack = std::make_unique<JumpAttack>(*this);
		break;
	case EnemyAttackManager::ATTACK_TYPE::JUMP_CONSTANT:
		attack = std::make_unique<JumpAttackConstant>(*this);
		break;
	case EnemyAttackManager::ATTACK_TYPE::FOLLOW:
		attack = std::make_unique<FollowAttack>(*this);
		attack->SetTarget(target_);
		break;
	case EnemyAttackManager::ATTACK_TYPE::FALL_DOWN:
		attack = std::make_unique<FallDownAttack>(*this);
		break;
	case EnemyAttackManager::ATTACK_TYPE::CROSS_LINE:
		attack = std::make_unique<CrossAttack>(*this);
		attack->SetTarget(target_);
		break;
	case EnemyAttackManager::ATTACK_TYPE::THUNDER_AROUND:
		attack = std::make_unique<ThunderAroundAttack>(*this);
		attack->SetTarget(target_);
		break;
	case EnemyAttackManager::ATTACK_TYPE::WATER_SPRIT:
		attack = std::make_unique<WaterSpritAttack>(*this);
		break;
	case EnemyAttackManager::ATTACK_TYPE::MAX:
		break;
	default:
		break;
	}
	attackList_.push_back(std::move(attack));
}


void EnemyAttackManager::DeleteAttack(ATTACK_TYPE type)
{
	for (auto& attack : attackList_)
	{
		auto type = attack->GetMyType();
		if (type == type)
		{
			attack = nullptr;
		}
	}
	//nullptrの場所を開放する
	Utility::EraseVectorAllay(attackList_);
}

void EnemyAttackManager::AllDeleteAttack(void)
{
	attackList_.clear();
}

int EnemyAttackManager::GetAnimNumber(ATTACK_STATE state, ATTACK_TYPE type)
{
	return enemy_.GetModelType().GetAnimType(state, type);
}

bool EnemyAttackManager::CheckMove(void)
{
	for (auto& attack : attackList_)
	{
		//稼働中のものがあるかを調べる
		if (!(attack->GetState() == AttackBase::STATE::NONE || attack->GetState() == AttackBase::STATE::FINISH))
		{
			return true;
		}
	}
	return false;
}

void EnemyAttackManager::PlayAttack(float dis)
{
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
		if (static_cast<int>(priorityAttackList.size()) == 0)return;
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
}

AnimationController& EnemyAttackManager::GetAnimController(void)
{
	return enemy_.GetAnimController();
}

std::weak_ptr<Transform> EnemyAttackManager::GetTransform(void)
{
	return enemy_.GetTransform();
}

Gravity& EnemyAttackManager::GetGravity(void)
{
	return enemy_.GetGravity();
}
