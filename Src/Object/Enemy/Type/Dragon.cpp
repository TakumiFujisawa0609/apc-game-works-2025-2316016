#include "../../../Manager/ResourceManager.h"
#include "../../Common/AnimationController.h"
#include "../../Common/Geometry/Triangle3D.h"
#include "../../Common/Geometry/Capsule.h"
#include "../EnemyBase.h"
#include "Dragon.h"


Dragon::Dragon(EnemyBase& parent):parent_(parent)
{
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
	framePos_.clear();
	animInfoMap_.clear();
}

void Dragon::Init(void)
{
}

void Dragon::Update(void)
{
	animCtrl_->Update();
	UpdateFramePos();
}

void Dragon::Draw(void)
{
}

void Dragon::UIDraw(void)
{
}

void Dragon::OnHit(const std::weak_ptr<Collider> _hitCol, VECTOR hitPos)
{
	parent_.OnHit(_hitCol, hitPos);
}

void Dragon::SetAnim(ANIM_TYPE type)
{
	animCtrl_->Play((int)type);
}

int Dragon::GetAnimType(EnemyAttackManager::ATTACK_STATE attackState, EnemyAttackManager::ATTACK_TYPE attackType)
{
	//ANIM_INFO info = {};
	//info.attackState = attackState;
	//info.attackType = attackType;
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

void Dragon::InitAnimationController(void)
{
	animCtrl_ = std::make_unique<AnimationController>(transform_->modelId);
	for (int i = 0; i < (int)ANIM_TYPE::MAX; i++)
	{
		animCtrl_->Add(i, 30.0f);
	}
	SetAnim(ANIM_TYPE::FLY_FORWARD);
}

void Dragon::InitGeometry(void)
{
	Collider::TAG tag = Collider::TAG::ENEMY;
	std::vector<Collider::TAG> notHitTags;
	notHitTags.push_back(Collider::TAG::ENEMY);
	notHitTags.push_back(Collider::TAG::ENEMY_ATTACK);
	//âHÇÃìñÇΩÇËîªíË
	std::unique_ptr<Geometry> geo = std::make_unique<Triangle3D>(framePos_[WING_L_BASE_BORN_NUM], framePos_[WING_L_1_BORN_NUM], framePos_[WING_L_2_BORN_NUM]);
	MakeCollider(tag, std::move(geo), notHitTags);
	geo = std::make_unique<Triangle3D>(framePos_[WING_L_BASE_BORN_NUM], framePos_[WING_L_2_BORN_NUM], framePos_[WING_L_3_BORN_NUM]);
	MakeCollider(tag, std::move(geo), notHitTags);
	geo = std::make_unique<Triangle3D>(framePos_[WING_L_BASE_BORN_NUM], framePos_[WING_L_3_BORN_NUM], framePos_[WING_L_4_BORN_NUM]);
	MakeCollider(tag, std::move(geo), notHitTags);
	geo = std::make_unique<Triangle3D>(framePos_[WING_L_BASE_BORN_NUM], framePos_[WING_L_4_BORN_NUM], framePos_[WING_L_5_BORN_NUM]);
	MakeCollider(tag, std::move(geo), notHitTags);
	geo = std::make_unique<Triangle3D>(framePos_[WING_L_BASE_BORN_NUM], framePos_[WING_L_5_BORN_NUM], framePos_[WING_L_NEAR_BODY_BORN_NUM]);
	MakeCollider(tag, std::move(geo), notHitTags);
	geo = std::make_unique<Triangle3D>(framePos_[WING_R_BASE_BORN_NUM], framePos_[WING_R_1_BORN_NUM], framePos_[WING_R_2_BORN_NUM]);
	MakeCollider(tag, std::move(geo), notHitTags);
	geo = std::make_unique<Triangle3D>(framePos_[WING_R_BASE_BORN_NUM], framePos_[WING_R_2_BORN_NUM], framePos_[WING_R_3_BORN_NUM]);
	MakeCollider(tag, std::move(geo), notHitTags);
	geo = std::make_unique<Triangle3D>(framePos_[WING_R_BASE_BORN_NUM], framePos_[WING_R_3_BORN_NUM], framePos_[WING_R_4_BORN_NUM]);
	MakeCollider(tag, std::move(geo), notHitTags);
	geo = std::make_unique<Triangle3D>(framePos_[WING_R_BASE_BORN_NUM], framePos_[WING_R_4_BORN_NUM], framePos_[WING_R_5_BORN_NUM]);
	MakeCollider(tag, std::move(geo), notHitTags);
	geo = std::make_unique<Triangle3D>(framePos_[WING_R_BASE_BORN_NUM], framePos_[WING_R_5_BORN_NUM], framePos_[WING_R_NEAR_BODY_BORN_NUM]);
	MakeCollider(tag, std::move(geo), notHitTags);
	//ì∑ëÃÇÃìñÇΩÇËîªíË
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

void Dragon::UpdateFramePos(void)
{
	int modelId = transform_->modelId;
	for (auto& framePos : framePos_)
	{
		framePos.second = MV1GetFramePosition(modelId, framePos.first);
	}
}

void Dragon::InitAnimMap(void)
{
	AddAnimInfoMap(EnemyAttackManager::ATTACK_STATE::READY, EnemyAttackManager::ATTACK_TYPE::JUMP,ANIM_TYPE::TAKE_OFF);
	AddAnimInfoMap(EnemyAttackManager::ATTACK_STATE::READY, EnemyAttackManager::ATTACK_TYPE::JUMP_CONSTANT, ANIM_TYPE::LAND);
	AddAnimInfoMap(EnemyAttackManager::ATTACK_STATE::READY, EnemyAttackManager::ATTACK_TYPE::FOLLOW, ANIM_TYPE::DEFEND);
	AddAnimInfoMap(EnemyAttackManager::ATTACK_STATE::READY, EnemyAttackManager::ATTACK_TYPE::FALL_DOWN, ANIM_TYPE::LAND);
	AddAnimInfoMap(EnemyAttackManager::ATTACK_STATE::READY, EnemyAttackManager::ATTACK_TYPE::CROSS_LINE, ANIM_TYPE::FLY_GLIDE);
	AddAnimInfoMap(EnemyAttackManager::ATTACK_STATE::READY, EnemyAttackManager::ATTACK_TYPE::THUNDER_AROUND, ANIM_TYPE::TAKE_OFF);
	AddAnimInfoMap(EnemyAttackManager::ATTACK_STATE::READY, EnemyAttackManager::ATTACK_TYPE::WATER_SPRIT, ANIM_TYPE::FLY_FLAME);

	AddAnimInfoMap(EnemyAttackManager::ATTACK_STATE::PLAY, EnemyAttackManager::ATTACK_TYPE::JUMP, ANIM_TYPE::SCREAM);
	AddAnimInfoMap(EnemyAttackManager::ATTACK_STATE::PLAY, EnemyAttackManager::ATTACK_TYPE::JUMP_CONSTANT, ANIM_TYPE::SCREAM);
	AddAnimInfoMap(EnemyAttackManager::ATTACK_STATE::PLAY, EnemyAttackManager::ATTACK_TYPE::FOLLOW, ANIM_TYPE::FLY_FORWARD);
	AddAnimInfoMap(EnemyAttackManager::ATTACK_STATE::PLAY, EnemyAttackManager::ATTACK_TYPE::FALL_DOWN, ANIM_TYPE::FLAME_ATTACK);
	AddAnimInfoMap(EnemyAttackManager::ATTACK_STATE::PLAY, EnemyAttackManager::ATTACK_TYPE::CROSS_LINE, ANIM_TYPE::SCREAM);
	AddAnimInfoMap(EnemyAttackManager::ATTACK_STATE::PLAY, EnemyAttackManager::ATTACK_TYPE::THUNDER_AROUND, ANIM_TYPE::FLY_FLAME);
	AddAnimInfoMap(EnemyAttackManager::ATTACK_STATE::PLAY, EnemyAttackManager::ATTACK_TYPE::WATER_SPRIT, ANIM_TYPE::FLAME_ATTACK);
	
}

void Dragon::AddAnimInfoMap(EnemyAttackManager::ATTACK_STATE attackState, EnemyAttackManager::ATTACK_TYPE attackType,ANIM_TYPE animType)
{
	ANIM_INFO animInfo;
	animInfo.attackState = attackState;
	animInfo.attackType = attackType;
	animInfoMap_[animInfo] = animType;
}

//const bool Dragon::ANIM_INFO::operator>(const ANIM_INFO& info)
//{
//	int myState = static_cast<int>(attackState);
//	int state = static_cast<int>(info.attackState);
//	if (myState != state)
//	{
//		return myState > state;
//	}
//	int myType = static_cast<int>(attackType);
//	int type = static_cast<int>(info.attackType);
//	return myType > type;
//}
//
//const bool Dragon::ANIM_INFO::operator<(const ANIM_INFO& info)
//{
//	int myState = static_cast<int>(attackState);
//	int state = static_cast<int>(info.attackState);
//	if (myState != state)
//	{
//		return myState < state;
//	}
//	int myType = static_cast<int>(attackType);
//	int type = static_cast<int>(info.attackType);
//	return myType < type;
//}
//
//const bool Dragon::ANIM_INFO::operator<=(const ANIM_INFO& info)
//{
//	int myState = static_cast<int>(attackState);
//	int state = static_cast<int>(info.attackState);
//	if (myState != state)
//	{
//		return myState <= state;
//	}
//	int myType = static_cast<int>(attackType);
//	int type = static_cast<int>(info.attackType);
//	return myType <= type;
//}
//
//const bool Dragon::ANIM_INFO::operator>=(const ANIM_INFO& info)
//{
//	int myState = static_cast<int>(attackState);
//	int state = static_cast<int>(info.attackState);
//	if (myState != state)
//	{
//		return myState >= state;
//	}
//	int myType = static_cast<int>(attackType);
//	int type = static_cast<int>(info.attackType);
//	return myType >= type;
//}
//
//const bool Dragon::ANIM_INFO::operator==(const ANIM_INFO& info)
//{
//	int myState = static_cast<int>(attackState);
//	int myType = static_cast<int>(attackType);
//	int state = static_cast<int>(info.attackState);
//	int type = static_cast<int>(info.attackType);
//	return myState == state && myType == type;
//}
