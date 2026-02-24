#include "../../../Manager/ResourceManager.h"
#include "../../Common/AnimationController.h"
#include "../../Common/Geometry/Triangle3D.h"
#include "../../Common/Geometry/Capsule.h"
#include "../EnemyBase.h"
#include "Dragon.h"

Dragon::Dragon(EnemyBase& parent):EnemyTypeBase(parent)
{
	type_ = TYPE::DRAGON;
	//ìGÇÃÉgÉâÉìÉXÉtÉHÅ[ÉÄÇé©ï™ÇÃÇ‡ÇÃÇ∆ÇµÇƒàµÇ§
	transform_ = parent_.GetTransform().lock();
	transform_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::DRAGON));
	transform_->scl = { MODEL_SIZE,MODEL_SIZE,MODEL_SIZE };
	transform_->Update();

	InitGeometry();
	InitAnimationController();
	InitAnimMap();
	animCtrl_->Play((int)ANIM_TYPE::IDLE_1);
}

Dragon::~Dragon(void)
{
}

void Dragon::InitAnimationController(void)
{
	animCtrl_ = std::make_unique<AnimationController>(transform_->modelId);
	for (int i = 0; i < (int)ANIM_TYPE::MAX; i++)
	{
		if (i == static_cast<int>(ANIM_TYPE::HIT))
		{
			animCtrl_->Add(i, 10.0f);
		}
		else
		{
			animCtrl_->Add(i, 30.0f);
		}
	}
	SetAnim((int)ANIM_TYPE::FLY_FORWARD);
}

void Dragon::InitGeometry(void)
{
	Collider::TAG tag = Collider::TAG::ENEMY_WING;
	std::vector<Collider::TAG> notHitTags;
	notHitTags.push_back(Collider::TAG::ENEMY);
	notHitTags.push_back(Collider::TAG::ENEMY_WING);
	notHitTags.push_back(Collider::TAG::ENEMY_ATTACK);
	std::vector<Collider::TAG> notHitTagWings;
	notHitTagWings.push_back(Collider::TAG::ENEMY);
	notHitTagWings.push_back(Collider::TAG::ENEMY_WING);
	notHitTagWings.push_back(Collider::TAG::ENEMY_ATTACK);
	notHitTagWings.push_back(Collider::TAG::PLAYER);
	notHitTagWings.push_back(Collider::TAG::PLAYER_LAND);

	//âHÇÃìñÇΩÇËîªíË
	std::unique_ptr<Geometry> geo = std::make_unique<Triangle3D>(framePos_[WING_L_BASE_BORN_NUM], framePos_[WING_L_1_BORN_NUM], framePos_[WING_L_2_BORN_NUM]);
	MakeCollider(tag, std::move(geo), notHitTagWings);
	geo = std::make_unique<Triangle3D>(framePos_[WING_L_BASE_BORN_NUM], framePos_[WING_L_2_BORN_NUM], framePos_[WING_L_3_BORN_NUM]);
	MakeCollider(tag, std::move(geo), notHitTagWings);
	geo = std::make_unique<Triangle3D>(framePos_[WING_L_BASE_BORN_NUM], framePos_[WING_L_3_BORN_NUM], framePos_[WING_L_4_BORN_NUM]);
	MakeCollider(tag, std::move(geo), notHitTagWings);
	geo = std::make_unique<Triangle3D>(framePos_[WING_L_BASE_BORN_NUM], framePos_[WING_L_4_BORN_NUM], framePos_[WING_L_5_BORN_NUM]);
	MakeCollider(tag, std::move(geo), notHitTagWings);
	geo = std::make_unique<Triangle3D>(framePos_[WING_L_BASE_BORN_NUM], framePos_[WING_L_5_BORN_NUM], framePos_[WING_L_NEAR_BODY_BORN_NUM]);
	MakeCollider(tag, std::move(geo), notHitTagWings);
	geo = std::make_unique<Triangle3D>(framePos_[WING_R_BASE_BORN_NUM], framePos_[WING_R_1_BORN_NUM], framePos_[WING_R_2_BORN_NUM]);
	MakeCollider(tag, std::move(geo), notHitTagWings);
	geo = std::make_unique<Triangle3D>(framePos_[WING_R_BASE_BORN_NUM], framePos_[WING_R_2_BORN_NUM], framePos_[WING_R_3_BORN_NUM]);
	MakeCollider(tag, std::move(geo), notHitTagWings);
	geo = std::make_unique<Triangle3D>(framePos_[WING_R_BASE_BORN_NUM], framePos_[WING_R_3_BORN_NUM], framePos_[WING_R_4_BORN_NUM]);
	MakeCollider(tag, std::move(geo), notHitTagWings);
	geo = std::make_unique<Triangle3D>(framePos_[WING_R_BASE_BORN_NUM], framePos_[WING_R_4_BORN_NUM], framePos_[WING_R_5_BORN_NUM]);
	MakeCollider(tag, std::move(geo), notHitTagWings);
	geo = std::make_unique<Triangle3D>(framePos_[WING_R_BASE_BORN_NUM], framePos_[WING_R_5_BORN_NUM], framePos_[WING_R_NEAR_BODY_BORN_NUM]);
	MakeCollider(tag, std::move(geo), notHitTagWings);
	//ì∑ëÃÇÃìñÇΩÇËîªíË
	tag = Collider::TAG::ENEMY;
	geo = std::make_unique<Capsule>(framePos_[BODY_1_BORN_NUM], framePos_[BODY_2_BORN_NUM], BIG_RADIUS);
	MakeCollider(tag, std::move(geo), notHitTags);
	//éÒÇÃìñÇΩÇËîªíË
	geo = std::make_unique<Capsule>(framePos_[BODY_2_BORN_NUM], framePos_[NECK_BORN_NUM], MIDIUM_RADIUS);
	MakeCollider(tag, std::move(geo), notHitTags);
	//éÒÇÃìñÇΩÇËîªíË
	geo = std::make_unique<Capsule>(framePos_[HEAD_BORN_NUM], framePos_[NECK_BORN_NUM], MIDIUM_RADIUS);
	MakeCollider(tag, std::move(geo), notHitTags);
	//êKîˆÇÃìñÇΩÇËîªíË
	geo = std::make_unique<Capsule>(framePos_[BODY_1_BORN_NUM], framePos_[TAIL_1_BORN_NUM], SMALL_RADIUS);
	MakeCollider(tag, std::move(geo), notHitTags);
	geo = std::make_unique<Capsule>(framePos_[TAIL_2_BORN_NUM], framePos_[TAIL_1_BORN_NUM], SMALL_RADIUS);
	MakeCollider(tag, std::move(geo), notHitTags);
	//òrãrÇÃìñÇΩÇËîªíË
	geo = std::make_unique<Capsule>(framePos_[BODY_1_BORN_NUM], framePos_[FOOT_L_BORN_NUM], SMALL_RADIUS);
	MakeCollider(tag, std::move(geo), notHitTags);
	geo = std::make_unique<Capsule>(framePos_[BODY_1_BORN_NUM], framePos_[FOOT_R_BORN_NUM], SMALL_RADIUS);
	MakeCollider(tag, std::move(geo), notHitTags);
	geo = std::make_unique<Capsule>(framePos_[LEG_L_BORN_NUM], framePos_[FOOT_L_BORN_NUM], SMALL_RADIUS);
	MakeCollider(tag, std::move(geo), notHitTags);
	geo = std::make_unique<Capsule>(framePos_[LEG_R_BORN_NUM], framePos_[FOOT_R_BORN_NUM], SMALL_RADIUS);
	MakeCollider(tag, std::move(geo), notHitTags);
	geo = std::make_unique<Capsule>(framePos_[BODY_2_BORN_NUM], framePos_[ARM_L_BORN_NUM], SMALL_RADIUS);
	MakeCollider(tag, std::move(geo), notHitTags);
	geo = std::make_unique<Capsule>(framePos_[BODY_2_BORN_NUM], framePos_[ARM_R_BORN_NUM], SMALL_RADIUS);
	MakeCollider(tag, std::move(geo), notHitTags);
	geo = std::make_unique<Capsule>(framePos_[HAND_L_BORN_NUM], framePos_[ARM_L_BORN_NUM], SMALL_RADIUS);
	MakeCollider(tag, std::move(geo), notHitTags);
	geo = std::make_unique<Capsule>(framePos_[HAND_R_BORN_NUM], framePos_[ARM_R_BORN_NUM], SMALL_RADIUS);
	MakeCollider(tag, std::move(geo), notHitTags);
}

void Dragon::InitAnimMap(void)
{
	AddAnimInfoMap(EnemyAttackManager::ATTACK_STATE::READY, EnemyAttackManager::ATTACK_TYPE::JUMP,(int)ANIM_TYPE::TAKE_OFF);
	AddAnimInfoMap(EnemyAttackManager::ATTACK_STATE::READY, EnemyAttackManager::ATTACK_TYPE::JUMP_CONSTANT,(int) ANIM_TYPE::LAND);
	AddAnimInfoMap(EnemyAttackManager::ATTACK_STATE::READY, EnemyAttackManager::ATTACK_TYPE::FOLLOW,(int) ANIM_TYPE::DEFEND);
	AddAnimInfoMap(EnemyAttackManager::ATTACK_STATE::READY, EnemyAttackManager::ATTACK_TYPE::FALL_DOWN,(int) ANIM_TYPE::LAND);
	AddAnimInfoMap(EnemyAttackManager::ATTACK_STATE::READY, EnemyAttackManager::ATTACK_TYPE::CROSS_LINE,(int) ANIM_TYPE::FLY_GLIDE);
	AddAnimInfoMap(EnemyAttackManager::ATTACK_STATE::READY, EnemyAttackManager::ATTACK_TYPE::THUNDER_AROUND,(int) ANIM_TYPE::TAKE_OFF);
	AddAnimInfoMap(EnemyAttackManager::ATTACK_STATE::READY, EnemyAttackManager::ATTACK_TYPE::WATER_SPRIT,(int) ANIM_TYPE::FLY_FLAME);

	AddAnimInfoMap(EnemyAttackManager::ATTACK_STATE::PLAY, EnemyAttackManager::ATTACK_TYPE::JUMP,(int) ANIM_TYPE::SCREAM);
	AddAnimInfoMap(EnemyAttackManager::ATTACK_STATE::PLAY, EnemyAttackManager::ATTACK_TYPE::JUMP_CONSTANT,(int) ANIM_TYPE::SCREAM);
	AddAnimInfoMap(EnemyAttackManager::ATTACK_STATE::PLAY, EnemyAttackManager::ATTACK_TYPE::FOLLOW,(int) ANIM_TYPE::FLY_FORWARD);
	AddAnimInfoMap(EnemyAttackManager::ATTACK_STATE::PLAY, EnemyAttackManager::ATTACK_TYPE::FALL_DOWN,(int) ANIM_TYPE::FLAME_ATTACK);
	AddAnimInfoMap(EnemyAttackManager::ATTACK_STATE::PLAY, EnemyAttackManager::ATTACK_TYPE::CROSS_LINE,(int) ANIM_TYPE::SCREAM);
	AddAnimInfoMap(EnemyAttackManager::ATTACK_STATE::PLAY, EnemyAttackManager::ATTACK_TYPE::THUNDER_AROUND,(int) ANIM_TYPE::FLY_FLAME);
	AddAnimInfoMap(EnemyAttackManager::ATTACK_STATE::PLAY, EnemyAttackManager::ATTACK_TYPE::WATER_SPRIT,(int) ANIM_TYPE::FLAME_ATTACK);
	
}

