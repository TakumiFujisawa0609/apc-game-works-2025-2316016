#include "../../../Utility/Utility.h"
#include "../../../Manager/ResourceManager.h"
#include "../../Common/AnimationController.h"
#include "../../Common/Geometry/Sphere.h"
#include "../../Common/Geometry/Capsule.h"
#include "../EnemyBase.h"
#include "Chicken.h"

Chicken::Chicken(EnemyBase& parent) :EnemyTypeBase(parent)
{
	type_ = TYPE::CHICKEN;
	//敵のトランスフォームを自分のものとして扱う
	transform_ = parent_.GetTransform().lock();
	transform_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::CHICKIN));
	transform_->scl = { MODEL_SIZE,MODEL_SIZE,MODEL_SIZE };
	transform_->Update();

	InitGeometry();
	InitAnimationController();
	InitAnimMap();
	animCtrl_->Play((int)ANIM_TYPE::IDLE);

	int TriangleListNum = MV1GetTriangleListNum(transform_->modelId);
	for (int i = 0; i < TriangleListNum; i++)
	{
		int b = MV1GetTriangleListVertexType(transform_->modelId, i);
		int c = MV1GetTriangleListPolygonNum(transform_->modelId, i);
		int d = MV1GetTriangleListVertexNum(transform_->modelId, i);
		int a = 0;
	}
	DX_MV1_VERTEX_TYPE_1FRAME;
}

Chicken::~Chicken(void)
{
}

void Chicken::InitGeometry(void)
{
	Collider::TAG tag = Collider::TAG::ENEMY;
	std::vector<Collider::TAG> notHitTags;
	notHitTags.push_back(Collider::TAG::ENEMY);
	notHitTags.push_back(Collider::TAG::ENEMY_ATTACK);
	std::unique_ptr<Geometry> geo;
	MV1GetFramePosition(transform_->modelId, CENTER_BORN_NUM);
	//胴体の当たり判定
	geo = std::make_unique<Sphere>(framePos_[CENTER_BORN_NUM], static_cast<float>(Utility::Distance(MV1GetFramePosition(transform_->modelId, CENTER_BORN_NUM), MV1GetFramePosition(transform_->modelId, ARM_L_S_BORN_NUM))));
	MakeCollider(tag, std::move(geo), notHitTags);
	//足の当たり判定
	geo = std::make_unique<Capsule>(framePos_[LEG_L_S_BORN_NUM], framePos_[LEG_L_C_BORN_NUM], LEG_RADIUS);
	MakeCollider(tag, std::move(geo), notHitTags);
	geo = std::make_unique<Capsule>(framePos_[LEG_L_C_BORN_NUM], framePos_[LEG_L_E_BORN_NUM], LEG_RADIUS);
	MakeCollider(tag, std::move(geo), notHitTags);
	geo = std::make_unique<Capsule>(framePos_[LEG_R_S_BORN_NUM], framePos_[LEG_R_C_BORN_NUM], LEG_RADIUS);
	MakeCollider(tag, std::move(geo), notHitTags);
	geo = std::make_unique<Capsule>(framePos_[LEG_R_C_BORN_NUM], framePos_[LEG_R_E_BORN_NUM], LEG_RADIUS);
	MakeCollider(tag, std::move(geo), notHitTags);
}

void Chicken::InitAnimationController(void)
{
	animCtrl_ = std::make_unique<AnimationController>(transform_->modelId);
	for (int i = 0; i < (int)ANIM_TYPE::MAX; i++)
	{
		animCtrl_->Add(i, 30.0f);
	}
	SetAnim((int)ANIM_TYPE::IDLE);
}

void Chicken::InitAnimMap(void)
{
}
