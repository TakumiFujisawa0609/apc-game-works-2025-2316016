#include "../../Utility/Utility.h"
#include "../../Manager/ResourceManager.h"
#include "../../Renderer/ModelMaterial.h"
#include "../../Renderer/ModelRenderer.h"
#include "../Common/Gravity.h"
#include "../Common/Transform.h"
#include "../Common/EffectController.h"
#include "../Common/Geometry/Line3D.h"
#include "PlayerShot.h"

PlayerShot::PlayerShot(VECTOR pPos, VECTOR tPos)
{
	auto& res = ResourceManager::GetInstance();
	transform_ = std::make_shared<Transform>();
	transform_->SetModel(res.LoadModelDuplicate(ResourceManager::SRC::SHOT));
	transform_->pos = pPos;
	transform_->scl = VGet(MODEL_SCL, MODEL_SCL, MODEL_SCL);
	transform_->Update();
	material_ = std::make_unique<ModelMaterial>(
		"NoTextureVS.cso", 0,
		"NoTexturePS.cso", 1
	);
	material_->AddConstBufPS(static_cast<FLOAT4>(COLOR));
	renderer_ = std::make_shared<ModelRenderer>(
		transform_->modelId, *material_
	);
	startPos_ = pPos;
	targetPos_ = tPos;
	dir_ = VSub(targetPos_, startPos_);
	gravity_ = std::make_unique<Gravity>();
	gravity_->Init();
	gravity_->SetDir(VGet(0.0f, -1.0f, 0.0f));
	gravity_->SetInitPower(POWER + (dir_.y < 0.0f?0.0f:dir_.y));
	dir_.y = 0.0f;
	dir_ = VNorm(dir_);
	effect_ = std::make_unique<EffectController>();
	effect_->Add(res.Load(ResourceManager::SRC::HIT_EFFECT).handleId_, EffectController::EFF_TYPE::HIT);
	isDead_ = false;
	state_ = STATE::SHOT;
	std::unique_ptr<Geometry> geo = std::make_unique<Line3D>(prePos_, transform_->pos);
	MakeCollider(Collider::TAG::PLAYER_ATTACK, std::move(geo), { Collider::TAG::PLAYER,Collider::TAG::PLAYER_ATTACK,Collider::TAG::ENEMY_ATTACK ,Collider::TAG::GATE});
}

PlayerShot::~PlayerShot()
{
}

void PlayerShot::Init(void)
{
}

void PlayerShot::Update(void)
{
	if (isDead_ == true)
	{
		return;
	}
	prePos_ = transform_->pos;
	switch (state_)
	{
	case PlayerShot::STATE::SHOT:
		//撃っている状態の更新
		//重力更新
		gravity_->Update();
		//方向を取得
		//座標の更新
		transform_->pos = VAdd(transform_->pos, VScale(dir_, SPEED));
		transform_->pos = VAdd(transform_->pos, VScale(gravity_->GetDir(), gravity_->GetPower()));
		transform_->Update();
		break;
	case PlayerShot::STATE::BLAST:
		effect_->Update();
		if (effect_->IsEnd(EffectController::EFF_TYPE::HIT, effectNum_))
		{
			state_ = STATE::DEAD;
		}
		break;
	case PlayerShot::STATE::DEAD:
		isDead_ = true;
		break;
	default:
		break;
	}

	if (transform_->pos.y < 0.0f)
	{
		isDead_ = true;
	}
}

void PlayerShot::Draw(void)
{
	if (isDead_ == true)
	{
		return;
	}
	switch (state_)
	{
	case PlayerShot::STATE::SHOT:
		//DrawSphere3D(transform_->pos, RADIUS, 16, GetColor(255, 255, 0), GetColor(255, 0, 0), true);
		renderer_->Draw();
		break;
	case PlayerShot::STATE::BLAST:
		
		break;
	case PlayerShot::STATE::DEAD:
		break;
	default:
		break;
	}
	
}

void PlayerShot::Hit(VECTOR hitPos,VECTOR  rot)
{
	state_ = STATE::BLAST;
	effectNum_ = effect_->Play(EffectController::EFF_TYPE::HIT, hitPos, Quaternion(rot), VGet(10.0f, 10.0f, 10.0f), false, 1.0f);
}

void PlayerShot::OnHit(const std::weak_ptr<Collider> _hitCol, VECTOR hitPos)
{
	state_ = STATE::BLAST;
	VECTOR dir = VSub(hitPos, prePos_);
	VECTOR rot = Utility::VECTOR_ZERO;
	rot.y = atan2f(dir.x, dir.z);
	effectNum_ = effect_->Play(EffectController::EFF_TYPE::HIT, hitPos, Quaternion(rot), VGet(10.0f, 10.0f, 10.0f), false, 1.0f);
	for (auto& colParam : colParam_)
	{
		colParam.collider_->Kill();
	}
}
