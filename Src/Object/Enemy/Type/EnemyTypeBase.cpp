#include "../../Common/AnimationController.h"
#include "../EnemyBase.h"
#include "EnemyTypeBase.h"

EnemyTypeBase::EnemyTypeBase(EnemyBase& parent) : parent_(parent)
{
}

EnemyTypeBase::~EnemyTypeBase(void)
{
	framePos_.clear();
	animInfoMap_.clear();
}

void EnemyTypeBase::Init(void)
{
}

void EnemyTypeBase::Update(void)
{
	animCtrl_->Update();
	UpdateFramePos();
}

void EnemyTypeBase::Draw(void)
{
	for(int i = 0; i < colParam_.size(); i++)
	{
		colParam_[i].collider_->GetGeometry().Draw();
	}
}

void EnemyTypeBase::UIDraw(void)
{
}

void EnemyTypeBase::OnHit(const std::weak_ptr<Collider> _hitCol, VECTOR hitPos)
{
	parent_.OnHit(_hitCol, hitPos);
}

void EnemyTypeBase::SetAnim(int type)
{
	animCtrl_->Play(type);
}

int EnemyTypeBase::GetAnimType(EnemyAttackManager::ATTACK_STATE attackState, EnemyAttackManager::ATTACK_TYPE attackType)
{
	for (auto& map : animInfoMap_)
	{
		auto info = map.first;
		if (info.attackState != attackState || info.attackType != attackType)
		{
			continue;
		}
		return static_cast<int>(map.second);
	}
	return -1;
}

void EnemyTypeBase::UpdateFramePos(void)
{
	int modelId = transform_->modelId;
	for (auto& framePos : framePos_)
	{
		framePos.second = MV1GetFramePosition(modelId, framePos.first);
	}
}

void EnemyTypeBase::AddAnimInfoMap(EnemyAttackManager::ATTACK_STATE attackState, EnemyAttackManager::ATTACK_TYPE attackType, int animType)
{
	ANIM_INFO animInfo;
	animInfo.attackState = attackState;
	animInfo.attackType = attackType;
	animInfoMap_[animInfo] = animType;
}