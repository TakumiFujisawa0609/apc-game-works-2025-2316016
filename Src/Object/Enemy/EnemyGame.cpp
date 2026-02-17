#include <cmath>
#include "../../Utility/Utility.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../Common/Gravity.h"
#include "../Common/AnimationController.h"
#include "../Player/PlayerBase.h"
#include "Type/Dragon.h"
#include "EnemyAttackManager.h"
#include "EnemyGame.h"

EnemyGame::EnemyGame(std::weak_ptr<Transform> target) : target_(target)
{
	wingDamageState_ = WING_DAMAGE_STATE::NONE;
	wingDamageNum_ = 0;
	wingHitDamage_ = WING_DAMAGE;
	bodyHitDamage_ = BODY_DAMAGE;
	disolve_ = INIT_DISOLVE;
	transform_ = std::make_shared<Transform>();
	enemyType_ = std::make_unique<Dragon>(*this);
	std::string vsName, psName;
	EnemyTypeBase::TYPE type = enemyType_->GetType();
	switch (type)
	{
	case EnemyTypeBase::TYPE::CHICKEN:
		vsName = "ChickenVS.cso";
		psName = "ChickenPS.cso";
		break;
	case EnemyTypeBase::TYPE::DRAGON:
		vsName = "DragonVS.cso";
		psName = "DragonPS.cso";
		break;
	case EnemyTypeBase::TYPE::MAX:
		break;
	default:
		break;
	}

	material_ = std::make_unique<ModelMaterial>(
		vsName, 0,
		psName, 5
	);
	//material_->AddConstBufVS({ TEXTURE_SCALE, 0.0f, 1.0f, 1.0f });
	material_->AddConstBufPS(static_cast<FLOAT4>(Utility::COLOR_F2FLOAT4(DEFAULT_COLOR)));
	material_->AddConstBufPS(static_cast<FLOAT4>(Utility::COLOR_F2FLOAT4(DAMAGE_COLOR_ADD)));
	material_->AddConstBufPS({ damageTime_, hitPos_.x, hitPos_.y, hitPos_.z });
	material_->AddConstBufPS({ DAMAGE_EFECT_RADIUS,0.0f, 0.0f, 0.0f });
	material_->AddConstBufPS({ disolve_,NOISE_SCALE, 0.0f, 0.0f });
	material_->SetTextureBuf(3, ResourceManager::GetInstance().Load(ResourceManager::SRC::NOISE).handleId_);
	renderer_ = std::make_shared<ModelRenderer>(
		transform_->modelId, *material_
	);
	damageTime_ = 0.0f;
	attackManager_ = std::make_unique<EnemyAttackManager>(*this);
	attackManager_->SetTarget(target_);
	hitPos_ = Utility::VECTOR_ZERO;
	gravity_ = std::make_unique<Gravity>();
	gravity_->Init();
	gravity_->SetDir(Utility::DIR_D);
	maxHP_ = 100.0f;
	hp_ = maxHP_;
	InitAddAttack();
	AplayChangeStateFunc();
	ChangeState(STATE::IDLE);
}

EnemyGame::~EnemyGame(void)
{
}

void EnemyGame::Init(void)
{
}

void EnemyGame::Update(void)
{
	damageTime_ -= SceneManager::GetInstance().GetDeltaTime();
	attackManager_->Update();
	updateState_();
	enemyType_->Update();
	gravity_->Update();
	AplayGravity();
	MoveLimit();
	VECTOR minPos, maxPos;
	transform_->Update();
	Utility::GetModelFlameBox(transform_->modelId, minPos, maxPos, { 0,1 });
	transform_->localPos.y -= minPos.y - transform_->pos.y;
	transform_->Update();
	material_->SetConstBufPS(2, { damageTime_, hitPos_.x, hitPos_.y, hitPos_.z });
}

void EnemyGame::Draw(void)
{
	//float size = 50.0f;
	//MV1DrawModel(transform_->modelId);
	renderer_->Draw();
	//enemyType_->Draw();
	for (auto& col : colParam_)
	{
		//col.geometry_->Draw();
	}
	attackManager_->Draw();

}

void EnemyGame::UIDraw(void)
{
	//animCtrl_->DebugDraw();
}

void EnemyGame::OnHit(const std::weak_ptr<Collider> _hitCol, VECTOR hitPos)
{
	std::shared_ptr<Collider> hitCol = _hitCol.lock();
	Collider::TAG tag = hitCol->GetTag();
	switch (tag)
	{
	case Collider::TAG::PLAYER_ATTACK:
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
	auto& colParam = enemyType_->GetColParams();
	for(auto& col : colParam)
	{
		if(!col.collider_->IsHit())
		{
			continue;
		}
		if (col.collider_->GetTag() == Collider::TAG::ENEMY)
		{
			Damage(bodyHitDamage_);
		}
		else
		{
			wingDamageNum_++;
			Damage(wingHitDamage_);
			UpdateWingDamageState();
		}
	}
	damageTime_ = DAMAGE_EFECT_TIME;
	hitPos_ = hitPos;
	material_->SetConstBufPS(2, { damageTime_, hitPos_.x, hitPos_.y, hitPos_.z });
}

void EnemyGame::Damage(float damage)
{
	hp_ -= damage;
}

void EnemyGame::MoveLimit(void)
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

void EnemyGame::AplayGravity(void)
{
	transform_->pos = VAdd(transform_->pos, VScale(gravity_->GetDir(), gravity_->GetPower()));
}

void EnemyGame::UpdateWingDamageState(void)
{
	auto& resManager = ResourceManager::GetInstance();
	int texIndex = MV1GetMaterialDifMapTexture(transform_->modelId, 0);
	if (wingDamageNum_ == WING_LOW_DAMAGE_NUM)
	{
		wingHitDamage_ *= WING_LOW_DAMAGE_RATE;
		wingDamageState_ = WING_DAMAGE_STATE::LOW;
		int damageTexId = resManager.Load(ResourceManager::SRC::DRAGON_LOW_DAMAGE_TEXTURE).handleId_;
		MV1SetTextureGraphHandle(transform_->modelId, texIndex, damageTexId, TRUE);
	}
	else if(wingDamageNum_ == WING_HIGH_DAMAGE_NUM)
	{
		wingHitDamage_ *= WING_HIGH_DAMAGE_RATE;
		bodyHitDamage_ *= WING_HIGH_DAMAGE_BODY_DAMAGE_RATE;
		wingDamageState_ = WING_DAMAGE_STATE::HIGH;
		int damageTexId = resManager.Load(ResourceManager::SRC::DRAGON_HIGH_DAMAGE_TEXTURE).handleId_;
		MV1SetTextureGraphHandle(transform_->modelId, texIndex, damageTexId, TRUE);
	}
}

void EnemyGame::ChangeStateIdle(void)
{
	updateState_ = std::bind(&EnemyGame::UpdateIdle, this);
	GetAnimController().Play(enemyType_->GetIdleAnim());
}

void EnemyGame::ChangeStateAttack(void)
{
	updateState_ = std::bind(&EnemyGame::UpdateAttack, this);
}

void EnemyGame::ChangeStateDead(void)
{
	updateState_ = std::bind(&EnemyGame::UpdateDead, this);
	attackManager_->AllDeleteAttack();
	GetAnimController().Play((int)Dragon::ANIM_TYPE::DIE,false);
	auto& colParam = enemyType_->GetColParams();
	for (auto& col : colParam)
	{
		col.collider_->Kill();
	}
}

void EnemyGame::UpdateIdle(void)
{
	float dis = Utility::MagnitudeF(VSub(transform_->pos, target_.lock()->pos));
	attackManager_->PlayAttack(dis);
	ChangeState(STATE::ATTACK);
}

void EnemyGame::UpdateAttack(void)
{
	if(hp_ <= 0.0f)
	{
		ChangeState(STATE::DEAD);
		return;
	}
	bool isMove = attackManager_->CheckMove() == true;
	if (isMove)
	{
		return;
	}
	//‰Ò“­’†‚Ì‚à‚Ì‚ª‚È‚¯‚ê‚ÎIDLE‚É–ß‚·
	ChangeState(STATE::IDLE);
}

void EnemyGame::UpdateDead(void)
{
	enemyType_->Update();
	auto& animCtrl = enemyType_->GetAnimController();
	if (animCtrl.IsEnd())
	{
		float delta = SceneManager::GetInstance().GetDeltaTime();
		disolve_ += DISOLVE_MAX * delta / DISOLVE_TIME;
		material_->SetConstBufPS(4, { disolve_, NOISE_SCALE, 0.0f, 0.0f });
	}
	if (disolve_ >= DISOLVE_MAX)
	{
		isEnd_ = true;
	}
}

void EnemyGame::InitAddAttack(void)
{
	attackManager_->AddAttack(EnemyAttackManager::ATTACK_TYPE::JUMP);
	attackManager_->AddAttack(EnemyAttackManager::ATTACK_TYPE::JUMP_CONSTANT);
	attackManager_->AddAttack(EnemyAttackManager::ATTACK_TYPE::FOLLOW);
	attackManager_->AddAttack(EnemyAttackManager::ATTACK_TYPE::FALL_DOWN);
	attackManager_->AddAttack(EnemyAttackManager::ATTACK_TYPE::CROSS_LINE);
	attackManager_->AddAttack(EnemyAttackManager::ATTACK_TYPE::THUNDER_AROUND);
	attackManager_->AddAttack(EnemyAttackManager::ATTACK_TYPE::WATER_SPRIT);
}

void EnemyGame::AplayChangeStateFunc(void)
{
	changeState_[(STATE::IDLE)] = std::bind(&EnemyGame::ChangeStateIdle, this);
	changeState_[(STATE::ATTACK)] = std::bind(&EnemyGame::ChangeStateAttack, this);
	changeState_[(STATE::DEAD)] = std::bind(&EnemyGame::ChangeStateDead, this);
}
