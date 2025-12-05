#include <cmath>
#include "../../Utility/Utility.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../Common/Gravity.h"
#include "../Common/AnimationController.h"
#include "../Player/PlayerBase.h"
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

EnemyBase::EnemyBase(std::weak_ptr<Transform> target) : target_(target)
{
	damageTime_ = 0.0f;
	hitPos_ = Utility::VECTOR_ZERO;
	transform_ = std::make_shared<Transform>();
	dragon_ = std::make_unique<Dragon>(*this);

	material_ = std::make_unique<ModelMaterial>(
		"EnemyVS.cso", 0,
		"EnemyPS.cso", 4
	);
	//material_->AddConstBufVS({ TEXTURE_SCALE, 0.0f, 1.0f, 1.0f });
	material_->AddConstBufPS(static_cast<FLOAT4>(Utility::COLOR_F2FLOAT4(DEFAULT_COLOR)));
	material_->AddConstBufPS(static_cast<FLOAT4>(Utility::COLOR_F2FLOAT4(DAMAGE_COLOR_ADD)));
	material_->AddConstBufPS({damageTime_, hitPos_.x, hitPos_.y, hitPos_.z});
	material_->AddConstBufPS({DAMAGE_EFECT_RADIUS,0.0f, 0.0f, 0.0f});
	//material_->SetTextureBuf(3, ResourceManager::GetInstance().Load(ResourceManager::SRC::NOISE).handleId_);
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
	attackList_.clear();
}

void EnemyBase::Init(void)
{
}

void EnemyBase::Update(void)
{
	damageTime_ -= SceneManager::GetInstance().GetDeltaTime();
	for (auto& attack : attackList_  )
	{
		attack->Update();
	}
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

	for (auto& attack : attackList_)
	{
		attack->Draw();
	}
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
	case EnemyBase::ATTACK_TYPE::JUMP_CONSTANT:
		attack = std::make_unique<JumpAttackConstant>(*this);
		break;
	case EnemyBase::ATTACK_TYPE::FOLLOW:
		attack = std::make_unique<FollowAttack>(*this);
		attack->SetTarget(target_);
		break;
	case EnemyBase::ATTACK_TYPE::FALL_DOWN:
		attack = std::make_unique<FallDownAttack>(*this);
		break;
	case EnemyBase::ATTACK_TYPE::CROSS_LINE:
		attack = std::make_unique<CrossAttack>(*this);
		attack->SetTarget(target_);
		break;
	case EnemyBase::ATTACK_TYPE::THUNDER_AROUND:
		attack = std::make_unique<ThunderAroundAttack>(*this);
		attack->SetTarget(target_);
		break;
	case EnemyBase::ATTACK_TYPE::WATER_SPRIT:
		attack = std::make_unique<WaterSpritAttack>(*this);
		break;
	case EnemyBase::ATTACK_TYPE::MAX:
		break;
	default:
		break;
	}
	attackList_.push_back(std::move(attack));
}

void EnemyBase::DeleteAttack(ATTACK_TYPE type)
{
	for (auto& attack : attackList_)
	{
		auto type =attack->GetMyType();
		if (type == type)
		{
			attack = nullptr;
		}
	}
	//nullptrの場所を開放する
	Utility::EraseVectorAllay(attackList_);
}

void EnemyBase::AllDeleteAttack(void)
{
	attackList_.clear();
}

int EnemyBase::GetAnimNumber(ATTACK_STATE state, ATTACK_TYPE type)
{
	return dragon_->GetAnimType(state, type);
}

void EnemyBase::AplayChangeStateFunc(void)
{
	changeState_[(STATE::IDLE)] = std::bind(&EnemyBase::ChangeStateIdle, this);
	changeState_[(STATE::ATTACK)] = std::bind(&EnemyBase::ChangeStateAttack, this);
	changeState_[(STATE::DEAD)] = std::bind(&EnemyBase::ChangeStateDead, this);
}

void EnemyBase::InitAddAttack(void)
{
	//AddAttack(ATTACK_TYPE::JUMP);
	//AddAttack(ATTACK_TYPE::JUMP_CONSTANT);
	//AddAttack(ATTACK_TYPE::FOLLOW);
	//AddAttack(ATTACK_TYPE::FALL_DOWN);
	AddAttack(ATTACK_TYPE::CROSS_LINE);
	//AddAttack(ATTACK_TYPE::THUNDER_AROUND);
	//AddAttack(ATTACK_TYPE::WATER_SPRIT);
}

AnimationController& EnemyBase::GetAnimController(void)
{
	return dragon_->GetAnimController();
}
