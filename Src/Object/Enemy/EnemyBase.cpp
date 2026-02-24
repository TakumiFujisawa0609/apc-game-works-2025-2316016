#include <cmath>
#include "../../Utility/Utility.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../Common/Gravity.h"
#include "../Common/AnimationController.h"
#include "../Player/PlayerBase.h"
#include "Type/Dragon.h"
#include "EnemyAttackManager.h"
#include "EnemyBase.h"

EnemyBase::EnemyBase(void)
{
	damageTime_ = 0.0f;
	hitPos_ = Utility::VECTOR_ZERO;
}

EnemyBase::~EnemyBase(void)
{
}

void EnemyBase::Init(void)
{
}

void EnemyBase::Update(void)
{
	damageTime_ -= SceneManager::GetInstance().GetDeltaTime();
	enemyType_->Update();
	VECTOR minPos, maxPos;
	transform_->Update();
	Utility::GetModelFlameBox(transform_->modelId, minPos, maxPos,{0,1});
	transform_->localPos.y -= minPos.y - transform_->pos.y;
	transform_->Update();
	material_->SetConstBufPS(2,{ damageTime_, hitPos_.x, hitPos_.y, hitPos_.z });
}

void EnemyBase::Draw(void)
{
	//float size = 50.0f;
	//MV1DrawModel(transform_->modelId);
	renderer_->Draw();
	//enemyType_->Draw();

}

void EnemyBase::UIDraw(void)
{
	//animCtrl_->DebugDraw();
}

void EnemyBase::OnHit(const std::weak_ptr<Collider> _hitCol, VECTOR hitPos)
{
}


void EnemyBase::ChangeState(STATE state)
{
	state_ = state;
	changeState_[state]();
}

void EnemyBase::ChangeStateIdle(void)
{
	updateState_ = std::bind(&EnemyBase::UpdateIdle, this);
	GetAnimController().Play(enemyType_->GetIdleAnim());
}

void EnemyBase::ChangeStateAttack(void)
{
	updateState_ = std::bind(&EnemyBase::UpdateAttack, this);
}

void EnemyBase::ChangeStateBreak(void)
{
	updateState_ = std::bind(&EnemyBase::UpdateBreak, this);
}

void EnemyBase::ChangeStateDead(void)
{
	updateState_ = std::bind(&EnemyBase::UpdateDead, this);
}

void EnemyBase::UpdateIdle(void)
{

}

void EnemyBase::UpdateAttack(void)
{

}

void EnemyBase::UpdateBreak(void)
{
}

void EnemyBase::UpdateDead(void)
{
}

void EnemyBase::AplayChangeStateFunc(void)
{
	changeState_[(STATE::IDLE)] = std::bind(&EnemyBase::ChangeStateIdle, this);
	changeState_[(STATE::ATTACK)] = std::bind(&EnemyBase::ChangeStateAttack, this);
	changeState_[(STATE::DEAD)] = std::bind(&EnemyBase::ChangeStateDead, this);
}

AnimationController& EnemyBase::GetAnimController(void)
{
	return enemyType_->GetAnimController();
}
