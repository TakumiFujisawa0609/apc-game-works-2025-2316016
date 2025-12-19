#include "../../Manager/SceneManager.h"
#include "../Common/AnimationController.h"
#include "Type/Dragon.h"
#include "EnemyOver.h"

EnemyOver::EnemyOver(std::weak_ptr<Transform> trans) : EnemyBase(trans)
{
	auto& animCtrl = dragon_->GetAnimController();
	animCtrl.Play((int)Dragon::ANIM_TYPE::CLAW_ATTACK, true);
	disolve_ = INIT_DISOLVE;
	renderer_->SetBuckCull(true);
}

EnemyOver::~EnemyOver(void)
{
}

void EnemyOver::Init(void)
{
}

void EnemyOver::Update(void)
{
	dragon_->Update();
}

void EnemyOver::Draw(void)
{
	EnemyBase::Draw();
}

void EnemyOver::UIDraw(void)
{
}

void EnemyOver::SetPos(VECTOR pos)
{
	transform_->pos = pos;
	transform_->Update();
}

void EnemyOver::SetRot(VECTOR rot)
{
	transform_->quaRot = Quaternion(rot);
	transform_->Update();
}

void EnemyOver::SetScl(VECTOR scl)
{
	transform_->scl = scl;
	transform_->Update();
}

void EnemyOver::InitAddAttack(void)
{
}
