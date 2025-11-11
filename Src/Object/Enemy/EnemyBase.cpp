#include <cmath>
#include <memory>
#include "../../Utility/Utility.h"
#include "../../Manager/ResourceManager.h"
#include "../../Renderer/ModelMaterial.h"
#include "../../Renderer/ModelRenderer.h"
#include "../Player/PlayerBase.h"
#include "../Common/Transform.h"
#include "../Common/Gravity.h"
#include "../Common/AnimationController.h"
#include "../Common/EffectController.h"
#include "../Common/Geometry/Triangle3D.h"
#include "../Common/Geometry/Sphere.h"
#include "../Common/Geometry/Capsule.h"
#include "Attack/Type/AttackBase.h"
#include "Attack/Type/JumpAttack.h"
#include "Attack/Type/JumpAttackConstant.h"
#include "Attack/Type/ThunderAroundAttack.h"
#include "Attack/Type/CrossAttack.h"
#include "Attack/Type/FollowAttack.h"
#include "Attack/Type/FallDownAttack.h"
#include "Attack/Type/WaterSpritAttack.h"
#include "EnemyBase.h"

EnemyBase::EnemyBase(Transform& target) : target_(target)
{
	transform_ = std::make_shared<Transform>();
	transform_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::DRAGON));
	transform_->scl = { MODEL_SIZE,MODEL_SIZE,MODEL_SIZE };
	transform_->Update();

	material_ = std::make_unique<ModelMaterial>(
		"EnemyVS.cso", 0,
		"EnemyPS.cso", 1
	);
	//material_->AddConstBufVS({ TEXTURE_SCALE, 0.0f, 1.0f, 1.0f });
	material_->AddConstBufPS(static_cast<FLOAT4>(Utility::COLOR_F2FLOAT4(DEFAULT_COLOR)));
	//material_->SetTextureBuf(3, ResourceManager::GetInstance().Load(ResourceManager::SRC::NOISE).handleId_);
	renderer_ = std::make_shared<ModelRenderer>(
		transform_->modelId, *material_
	);
	gravity_ = std::make_unique<Gravity>();
	gravity_->Init();
	gravity_->SetDir(Utility::DIR_D);
	maxHP_ = 100.0f;
	hp_ = maxHP_;
	InitAnimationControllerDragon();
	animCtrl_->Play((int)ANIM_TYPE_DRAGON::IDLE_1);
	AplayChangeStateFunc();
	InitAddAttack();
	InitFramePos();
	InitGeometry();
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
	for (auto& attack : attackList_  )
	{
		attack->Update();
	}
	updateState_();
	gravity_->Update();
	AplayGravity();
	MoveLimit();
	VECTOR minPos, maxPos;
	transform_->Update();
	Utility::GetModelFlameBox(transform_->modelId, minPos, maxPos,{0,1});
	transform_->localPos.y -= minPos.y - transform_->pos.y;
	transform_->Update();
	animCtrl_->Update();
	UpdateFramePos();
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
	case Collider::TAG::ENEMY:
	case Collider::TAG::ENEMY_ATTACK:
		return;
		break;
	default:
		break;
	}
	auto& hit = hitCol->GetParent();
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
	//óDêÊîÕàÕÇãÅÇﬂÇÈ
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
	//óDêÊìxÇ™çÇÇ¢Ç‡ÇÃÇäiî[Ç∑ÇÈ
	std::vector<std::unique_ptr<AttackBase>> priorityAttackList;
	for (auto& attack : attackList_)
	{
		//óDêÊãóó£ì‡Ç≈ÉNÅ[ÉãÉ^ÉCÉÄÇ™ñæÇØÇƒÇ¢ÇÈÇ‡ÇÃÇà⁄ìÆ
		auto range = attack->GetRange();
		if ((range == priorityRange || range == AttackBase::RANGE::ALL) && attack->GetState() == AttackBase::STATE::NONE)
		{
			priorityAttackList.push_back(std::move(attack));
		}
	}
	int size = static_cast<int>(priorityAttackList.size());
	if (size == 0)
	{
		//óDêÊãóó£äOÇ≈ÉNÅ[ÉãÉ^ÉCÉÄÇ™äJÇØÇƒÇ¢ÇÈÉÇÉmÇà⁄ìÆ
		for (auto& attack : attackList_)
		{
			if (attack->GetState() != AttackBase::STATE::NONE)continue;
			priorityAttackList.push_back(std::move(attack));
		}
		if(static_cast<int>(priorityAttackList.size()) == 0)return;
	}
	//nullptrÇÃèÍèäÇäJï˙Ç∑ÇÈ
	//std::erase_if(attackList_, [](auto& attack) {return attack == nullptr;});
	Utility::EraseVectorAllay(attackList_);
	//óDêÊìxÇ™ìØÇ∂Ç‡ÇÃÇÉâÉìÉ_ÉÄÇ≈ëIë
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
		//â“ì≠íÜÇÃÇ‡ÇÃÇ™Ç†ÇÈÇ©Çí≤Ç◊ÇÈ
		if (!(attack->GetState() == AttackBase::STATE::NONE || attack->GetState() == AttackBase::STATE::FINISH))
		{
			return;
		}
	}
	//â“ì≠íÜÇÃÇ‡ÇÃÇ™Ç»ÇØÇÍÇŒIDLEÇ…ñﬂÇ∑
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
		attack->SetTarget(&target_);
		break;
	case EnemyBase::ATTACK_TYPE::FALL_DOWN:
		attack = std::make_unique<FallDownAttack>(*this);
		break;
	case EnemyBase::ATTACK_TYPE::CROSS_LINE:
		attack = std::make_unique<CrossAttack>(*this);
		break;
	case EnemyBase::ATTACK_TYPE::THUNDER_AROUND:
		attack = std::make_unique<ThunderAroundAttack>(*this);
		attack->SetTarget(&target_);
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
	//nullptrÇÃèÍèäÇäJï˙Ç∑ÇÈ
	Utility::EraseVectorAllay(attackList_);
}

void EnemyBase::AllDeleteAttack(void)
{
	attackList_.clear();
}

void EnemyBase::SetAnim(ANIM_TYPE_DRAGON type)
{
	animCtrl_->Play((int)type);
}

void EnemyBase::AplayChangeStateFunc(void)
{
	changeState_[(STATE::IDLE)] = std::bind(&EnemyBase::ChangeStateIdle, this);
	changeState_[(STATE::ATTACK)] = std::bind(&EnemyBase::ChangeStateAttack, this);
	changeState_[(STATE::DEAD)] = std::bind(&EnemyBase::ChangeStateDead, this);
}

void EnemyBase::InitAnimationControllerDragon(void)
{
	animCtrl_ = std::make_unique<AnimationController>(transform_->modelId);
	for (int i = 0; i < (int)ANIM_TYPE_DRAGON::MAX;i++)
	{
		animCtrl_->Add(i, 30.0f);
	}
	SetAnim(ANIM_TYPE_DRAGON::FLY_FORWARD);
}

void EnemyBase::InitGeometry(void)
{
	Collider::TAG tag = Collider::TAG::ENEMY;
	std::vector<Collider::TAG> notHitTags;
	notHitTags.push_back(Collider::TAG::ENEMY);
	notHitTags.push_back(Collider::TAG::ENEMY_ATTACK);
	//âHÇÃìñÇΩÇËîªíË
	std::unique_ptr<Geometry> geo = std::make_unique<Triangle3D>(framePos_[WING_L_BASE_BORN_NUM], framePos_[WING_L_1_BORN_NUM], framePos_[WING_L_2_BORN_NUM]);
	MakeCollider(tag, std::move(geo), notHitTags);
	geo = std::make_unique<Triangle3D>( framePos_[WING_L_BASE_BORN_NUM], framePos_[WING_L_2_BORN_NUM], framePos_[WING_L_3_BORN_NUM]);
	MakeCollider(tag, std::move(geo), notHitTags);
	geo = std::make_unique<Triangle3D>( framePos_[WING_L_BASE_BORN_NUM], framePos_[WING_L_3_BORN_NUM], framePos_[WING_L_4_BORN_NUM]);
	MakeCollider(tag, std::move(geo), notHitTags);
	geo = std::make_unique<Triangle3D>( framePos_[WING_L_BASE_BORN_NUM], framePos_[WING_L_4_BORN_NUM], framePos_[WING_L_5_BORN_NUM]);
	MakeCollider(tag, std::move(geo), notHitTags);
	geo = std::make_unique<Triangle3D>( framePos_[WING_L_BASE_BORN_NUM], framePos_[WING_L_5_BORN_NUM], framePos_[WING_L_NEAR_BODY_BORN_NUM]);
	MakeCollider(tag, std::move(geo), notHitTags);
	geo = std::make_unique<Triangle3D>( framePos_[WING_R_BASE_BORN_NUM], framePos_[WING_R_1_BORN_NUM], framePos_[WING_R_2_BORN_NUM]);
	MakeCollider(tag, std::move(geo), notHitTags);
	geo = std::make_unique<Triangle3D>( framePos_[WING_R_BASE_BORN_NUM], framePos_[WING_R_2_BORN_NUM], framePos_[WING_R_3_BORN_NUM]);
	MakeCollider(tag, std::move(geo), notHitTags);
	geo = std::make_unique<Triangle3D>( framePos_[WING_R_BASE_BORN_NUM], framePos_[WING_R_3_BORN_NUM], framePos_[WING_R_4_BORN_NUM]);
	MakeCollider(tag, std::move(geo), notHitTags);
	geo = std::make_unique<Triangle3D>( framePos_[WING_R_BASE_BORN_NUM], framePos_[WING_R_4_BORN_NUM], framePos_[WING_R_5_BORN_NUM]);
	MakeCollider(tag, std::move(geo), notHitTags);
	geo = std::make_unique<Triangle3D>( framePos_[WING_R_BASE_BORN_NUM], framePos_[WING_R_5_BORN_NUM], framePos_[WING_R_NEAR_BODY_BORN_NUM]);
	MakeCollider(tag, std::move(geo), notHitTags);
	//ì∑ëÃÇÃìñÇΩÇËîªíË
	geo = std::make_unique<Capsule>( framePos_[BODY_1_BORN_NUM],framePos_[BODY_2_BORN_NUM],BIG_RADIUS);
	MakeCollider(tag, std::move(geo), notHitTags);
	//éÒÇÃìñÇΩÇËîªíË
	geo = std::make_unique<Capsule>( framePos_[BODY_2_BORN_NUM],framePos_[NECK_BORN_NUM],MIDIUM_RADIUS);
	MakeCollider(tag, std::move(geo), notHitTags);
	//éÒÇÃìñÇΩÇËîªíË
	geo = std::make_unique<Capsule>( framePos_[HEAD_BORN_NUM],framePos_[NECK_BORN_NUM],MIDIUM_RADIUS);
	MakeCollider(tag, std::move(geo), notHitTags);
	//êKîˆÇÃìñÇΩÇËîªíË
	geo = std::make_unique<Capsule>( framePos_[BODY_1_BORN_NUM],framePos_[TAIL_1_BORN_NUM],SMALL_RADIUS);
	MakeCollider(tag, std::move(geo), notHitTags);
	geo = std::make_unique<Capsule>( framePos_[TAIL_2_BORN_NUM],framePos_[TAIL_1_BORN_NUM],SMALL_RADIUS);
	MakeCollider(tag, std::move(geo), notHitTags);
	//òrãrÇÃìñÇΩÇËîªíË
	geo = std::make_unique<Capsule>( framePos_[BODY_1_BORN_NUM],framePos_[FOOT_L_BORN_NUM],SMALL_RADIUS);
	MakeCollider(tag, std::move(geo), notHitTags);
	geo = std::make_unique<Capsule>( framePos_[BODY_1_BORN_NUM],framePos_[FOOT_R_BORN_NUM],SMALL_RADIUS);
	MakeCollider(tag, std::move(geo), notHitTags);
	geo = std::make_unique<Capsule>( framePos_[LEG_L_BORN_NUM],framePos_[FOOT_L_BORN_NUM],SMALL_RADIUS);
	MakeCollider(tag, std::move(geo), notHitTags);
	geo = std::make_unique<Capsule>( framePos_[LEG_R_BORN_NUM],framePos_[FOOT_R_BORN_NUM],SMALL_RADIUS);
	MakeCollider(tag, std::move(geo), notHitTags);
	geo = std::make_unique<Capsule>( framePos_[BODY_2_BORN_NUM],framePos_[ARM_L_BORN_NUM],SMALL_RADIUS);
	MakeCollider(tag, std::move(geo), notHitTags);
	geo = std::make_unique<Capsule>( framePos_[BODY_2_BORN_NUM],framePos_[ARM_R_BORN_NUM],SMALL_RADIUS);
	MakeCollider(tag, std::move(geo), notHitTags);
	geo = std::make_unique<Capsule>( framePos_[HAND_L_BORN_NUM],framePos_[ARM_L_BORN_NUM],SMALL_RADIUS);
	MakeCollider(tag, std::move(geo), notHitTags);
	geo = std::make_unique<Capsule>( framePos_[HAND_R_BORN_NUM],framePos_[ARM_R_BORN_NUM],SMALL_RADIUS);
	MakeCollider(tag, std::move(geo), notHitTags);
}

void EnemyBase::InitAddAttack(void)
{
	AddAttack(ATTACK_TYPE::JUMP);
	AddAttack(ATTACK_TYPE::JUMP_CONSTANT);
	AddAttack(ATTACK_TYPE::FOLLOW);
	AddAttack(ATTACK_TYPE::FALL_DOWN);
	AddAttack(ATTACK_TYPE::CROSS_LINE);
	AddAttack(ATTACK_TYPE::THUNDER_AROUND);
	AddAttack(ATTACK_TYPE::WATER_SPRIT);
}

void EnemyBase::InitFramePos(void)
{
	//VECTOR initPos = Utility::VECTOR_ZERO;
	//framePos_[HAND_L_BORN_NUM] = initPos;
	//framePos_[HAND_R_BORN_NUM] = initPos;
	//framePos_[LEG_L_BORN_NUM] = initPos;
	//framePos_[LEG_R_BORN_NUM] = initPos;
	//framePos_[HEAD_BORN_NUM] = initPos;

	//framePos_[WING_L_BASE_BORN_NUM] = initPos;
	//framePos_[WING_L_1_BORN_NUM] = initPos;
	//framePos_[WING_L_2_BORN_NUM] = initPos;
	//framePos_[WING_L_3_BORN_NUM] = initPos;
	//framePos_[WING_L_4_BORN_NUM] = initPos;

	//framePos_[WING_R_BASE_BORN_NUM] = initPos;
	//framePos_[WING_R_1_BORN_NUM] = initPos;
	//framePos_[WING_R_2_BORN_NUM] = initPos;
	//framePos_[WING_R_3_BORN_NUM] = initPos;
	//framePos_[WING_R_4_BORN_NUM] = initPos;

	//framePos_[BODY_1_BORN_NUM] = initPos;
	//framePos_[BODY_2_BORN_NUM] = initPos;

	//framePos_[NECK_BORN_NUM] = initPos;
}

void EnemyBase::UpdateFramePos(void)
{
	int modelId = transform_->modelId;
	for (auto& framePos : framePos_)
	{
		framePos.second = MV1GetFramePosition(modelId, framePos.first);
	}
}
