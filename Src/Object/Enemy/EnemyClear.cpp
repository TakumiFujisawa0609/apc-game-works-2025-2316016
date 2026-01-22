#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../Common/AnimationController.h"
#include "Type/Dragon.h"
#include "EnemyClear.h"

EnemyClear::EnemyClear(void)
{
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
	material_->AddConstBufPS({ INIT_DISOLVE,NOISE_SCALE, 0.0f, 0.0f });
	material_->SetTextureBuf(3, ResourceManager::GetInstance().Load(ResourceManager::SRC::NOISE).handleId_);
	renderer_ = std::make_shared<ModelRenderer>(
		transform_->modelId, *material_
	);
	auto& animCtrl = enemyType_->GetAnimController();
	animCtrl.Play((int)Dragon::ANIM_TYPE::DIE, false);
	disolve_ = INIT_DISOLVE;
	renderer_->SetBuckCull(true);
	AplayChangeStateFunc();
	ChangeState(STATE::IDLE);
}

EnemyClear::~EnemyClear(void)
{
}

void EnemyClear::Init(void)
{
}

void EnemyClear::Update(void)
{
	enemyType_->Update();
	auto& animCtrl = enemyType_->GetAnimController();
	if (animCtrl.IsEnd())
	{
		float delta = SceneManager::GetInstance().GetDeltaTime();
		disolve_ += DISOLVE_MAX * delta / DISOLVE_TIME;
		material_->SetConstBufPS(4, { disolve_, NOISE_SCALE, 0.0f, 0.0f });
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
