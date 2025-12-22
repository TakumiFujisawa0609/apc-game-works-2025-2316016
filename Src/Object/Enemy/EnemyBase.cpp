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

EnemyBase::EnemyBase(std::weak_ptr<Transform> target) : target_(target)
{
	damageTime_ = 0.0f;
	attackManager_ = std::make_unique<EnemyAttackManager>(*this);
	attackManager_->SetTarget(target_);
	hitPos_ = Utility::VECTOR_ZERO;
	transform_ = std::make_shared<Transform>();
	dragon_ = std::make_unique<Dragon>(*this);

	material_ = std::make_unique<ModelMaterial>(
		"EnemyVS.cso", 0,
		"EnemyPS.cso", 5
	);
	//material_->AddConstBufVS({ TEXTURE_SCALE, 0.0f, 1.0f, 1.0f });
	material_->AddConstBufPS(static_cast<FLOAT4>(Utility::COLOR_F2FLOAT4(DEFAULT_COLOR)));
	material_->AddConstBufPS(static_cast<FLOAT4>(Utility::COLOR_F2FLOAT4(DAMAGE_COLOR_ADD)));
	material_->AddConstBufPS({damageTime_, hitPos_.x, hitPos_.y, hitPos_.z});
	material_->AddConstBufPS({DAMAGE_EFECT_RADIUS,0.0f, 0.0f, 0.0f});
	material_->AddConstBufPS({INIT_DISOLVE,NOISE_SCALE, 0.0f, 0.0f});
	material_->SetTextureBuf(3, ResourceManager::GetInstance().Load(ResourceManager::SRC::NOISE).handleId_);
	renderer_ = std::make_shared<ModelRenderer>(
		transform_->modelId, *material_
	);
	gravity_ = std::make_unique<Gravity>();
	gravity_->Init();
	gravity_->SetDir(Utility::DIR_D);
	maxHP_ = 100.0f;
	hp_ = maxHP_;
	AplayChangeStateFunc();
	InitAddAttack();
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
	damageTime_ -= SceneManager::GetInstance().GetDeltaTime();
	attackManager_->Update();
	updateState_();
	dragon_->Update();
	gravity_->Update();
	AplayGravity();
	MoveLimit();
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
	for (auto& col : colParam_)
	{
		//col.geometry_->Draw();
	}
	attackManager_->Draw();

}

void EnemyBase::UIDraw(void)
{
	//animCtrl_->DebugDraw();
}

void EnemyBase::OnHit(const std::weak_ptr<Collider> _hitCol, VECTOR hitPos)
{
	std::shared_ptr<Collider> hitCol = _hitCol.lock();
	Collider::TAG tag = hitCol->GetTag();
	switch (tag)
	{
	case Collider::TAG::PLAYER_ATTACK:
		Damage(2.0f);
		break;
	case Collider::TAG::PLAYER:
	case Collider::TAG::PLAYER_LAND:
	case Collider::TAG::ENEMY:
	case Collider::TAG::ENEMY_ATTACK:
		return;
		break;
	default:
		break;
	}
	//auto& hit = hitCol->GetParent();
	damageTime_ = DAMAGE_EFECT_TIME;
	hitPos_ = hitPos;
	material_->SetConstBufPS(2, { damageTime_, hitPos_.x, hitPos_.y, hitPos_.z });
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
	GetAnimController().Play(dragon_->GetIdleAnim());
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
	float dis = Utility::MagnitudeF(VSub(transform_->pos, target_.lock()->pos));
	attackManager_->PlayAttack(dis);
	ChangeState(STATE::ATTACK);
}

void EnemyBase::UpdateAttack(void)
{
	bool isMove =attackManager_->CheckMove() == true;
	if (isMove)
	{
		return;
	}
	//‰Ò“­’†‚Ì‚à‚Ì‚ª‚È‚¯‚ê‚ÎIDLE‚É–ß‚·
	ChangeState(STATE::IDLE);
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

void EnemyBase::InitAddAttack(void)
{
	attackManager_->AddAttack(EnemyAttackManager::ATTACK_TYPE::JUMP);
	attackManager_->AddAttack(EnemyAttackManager::ATTACK_TYPE::JUMP_CONSTANT);
	attackManager_->AddAttack(EnemyAttackManager::ATTACK_TYPE::FOLLOW);
	attackManager_->AddAttack(EnemyAttackManager::ATTACK_TYPE::FALL_DOWN);
	attackManager_->AddAttack(EnemyAttackManager::ATTACK_TYPE::CROSS_LINE);
	attackManager_->AddAttack(EnemyAttackManager::ATTACK_TYPE::THUNDER_AROUND);
	attackManager_->AddAttack(EnemyAttackManager::ATTACK_TYPE::WATER_SPRIT);
}

AnimationController& EnemyBase::GetAnimController(void)
{
	return dragon_->GetAnimController();
}
