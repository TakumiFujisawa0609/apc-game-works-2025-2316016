#include "../../Manager/SceneManager.h"
#include "../Common/AnimationController.h"
#include "Type/Dragon.h"
#include "EnemyClear.h"

EnemyClear::EnemyClear(std::weak_ptr<Transform> trans) : EnemyBase(trans)
{
	auto& animCtrl = dragon_->GetAnimController();
	animCtrl.Play((int)Dragon::ANIM_TYPE::DIE, false);
	disolve_ = INIT_DISOLVE;
	renderer_->SetBuckCull(true);
}

EnemyClear::~EnemyClear(void)
{
}

void EnemyClear::Init(void)
{
}

void EnemyClear::Update(void)
{
	dragon_->Update();
	auto& animCtrl = dragon_->GetAnimController();
	if (animCtrl.IsEnd())
	{
		float delta = SceneManager::GetInstance().GetDeltaTime();
		disolve_ += DISOLVE_MAX * delta / DISOLVE_TIME;
		material_->SetConstBufPS(4, { disolve_, 0.0f, 0.0f, 0.0f });
	}
}

void EnemyClear::Draw(void)
{
	EnemyBase::Draw();
}

void EnemyClear::UIDraw(void)
{
}

void EnemyClear::SetPos(VECTOR pos)
{
	transform_->pos = pos;
	transform_->Update();
}

void EnemyClear::SetRot(VECTOR rot)
{
	transform_->quaRot = Quaternion(rot);
	transform_->Update();
}

void EnemyClear::SetScl(VECTOR scl)
{
	transform_->scl = scl;
	transform_->Update();
}

void EnemyClear::InitAddAttack(void)
{
}
