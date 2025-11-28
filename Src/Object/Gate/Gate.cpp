#include "../../Manager/ResourceManager.h"
#include "../../Renderer/ModelMaterial.h"
#include "../../Renderer/ModelRenderer.h"
#include "../Common/Geometry/Triangle3D.h"
#include "../Common/Collider.h"
#include "GateMist.h"
#include "Gate.h"

Gate::Gate(SceneManager::SCENE_ID nextSceneID)
{
	VECTOR pos[GateMist::VERTEX_NUM];
	pos[0] = pos1_;
	pos[1] = pos2_;
	pos[2] = pos3_;
	pos[3] = pos4_;
	mist_ = std::make_unique<GateMist>(pos);
	nextSceneID_ = nextSceneID;
	transform_ = std::make_shared<Transform>();
	transform_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::GATE));
	transform_->scl = SCALE;
	transform_->Update();
	material_ = std::make_unique<ModelMaterial>(
		"GateVS.cso", 1,
		"GatePS.cso", 0
	);
	material_->AddConstBufVS({ TEXTURE_SCALE, 0.0f, 0.0f, 0.0f });
	renderer_ = std::make_shared<ModelRenderer>(
		transform_->modelId, *material_
	);
	InitCollider();
}

Gate::~Gate(void)
{
}

void Gate::Init(void)
{
}

void Gate::Update(void)
{
	mist_->Update();
}

void Gate::Draw(void)
{
	//MV1DrawModel(transform_->modelId);
	renderer_->Draw();
	mist_->Draw();
}

void Gate::UIDraw(void)
{
}

void Gate::OnHit(const std::weak_ptr<Collider> _hitCol, VECTOR hitPos)
{
	std::shared_ptr<Collider> hitCol = _hitCol.lock();
	auto tag = hitCol->GetTag();
	switch (tag)
	{
	case Collider::TAG::PLAYER:
		//ここにゲートに当たったときの処理を書く
		SceneManager::GetInstance().ChangeScene(nextSceneID_,true);
		break;
	case Collider::TAG::PLAYER_LAND:
	case Collider::TAG::PLAYER_ATTACK:
	case Collider::TAG::ENEMY:
	case Collider::TAG::ENEMY_ATTACK:
	case Collider::TAG::GATE:
		return;
		break;
	default:
		break;
	}
}

void Gate::SetPos(VECTOR pos)
{
	VECTOR distance = VSub(pos, transform_->pos);
	transform_->pos = pos;
	transform_->Update();
	pos1_ = VAdd(pos1_, distance);
	pos2_ = VAdd(pos2_, distance);
	pos3_ = VAdd(pos3_, distance);
	pos4_ = VAdd(pos4_, distance);

	SetMistPos();
}

void Gate::SetDegY(float degY)
{
	transform_->quaRot = Quaternion(VGet(0.0f, Utility::Deg2RadF(degY), 0.0f));
	transform_->Update();
	UpdatePos();
}

void Gate::InitCollider(void)
{
	UpdatePos();
	std::unique_ptr<Geometry>geo = std::make_unique<Triangle3D>(pos1_, pos2_, pos3_);
	MakeCollider(Collider::TAG::GATE, std::move(geo));
	geo = std::make_unique<Triangle3D>(pos4_, pos2_, pos3_);
	MakeCollider(Collider::TAG::GATE, std::move(geo));
}

void Gate::UpdatePos(void)
{
	pos1_ = MV1GetFramePosition(transform_->modelId, 0);	//左下
	pos2_ = MV1GetFramePosition(transform_->modelId, 1);
	pos3_ = VSub(pos1_, pos2_);
	pos3_.y *= -1.0f;
	pos3_ = VAdd(pos2_, VScale(pos3_, -1.0f));	//右下
	pos2_ = VGet(pos1_.x, pos2_.y, pos1_.z);//左上
	pos4_ = VGet(pos3_.x, pos2_.y, pos3_.z);	//右上

	SetMistPos();
}

void Gate::SetMistPos(void)
{
	VECTOR pos[GateMist::VERTEX_NUM];
	pos[0] = pos1_;
	pos[1] = pos2_;
	pos[2] = pos3_;
	pos[3] = pos4_;
	mist_->SetPos(pos);
}
