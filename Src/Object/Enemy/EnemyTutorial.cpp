#include "../../Utility/Utility.h"
#include "../../Manager/ResourceManager.h"
#include "../../Renderer/ModelMaterial.h"
#include "../../Renderer/ModelRenderer.h"
#include "../Common/EffectController.h"
#include "../Common/Transform.h"
#include "../Common/Gravity.h"
#include "../Common/AnimationController.h"
#include "Type/Chicken.h"
#include "EnemyTutorial.h"

EnemyTutorial::EnemyTutorial(std::weak_ptr<Transform> target) : EnemyGame(target)
{
	enemyType_ = std::make_unique<Chicken>(*this);
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
	InitAddAttack();
	AplayChangeStateFunc();
	ChangeState(STATE::IDLE);
}

EnemyTutorial::~EnemyTutorial()
{
}

void EnemyTutorial::Init(void)
{
}

void EnemyTutorial::Update(void)
{
}

void EnemyTutorial::Draw(void)
{
}

void EnemyTutorial::InitAddAttack(void)
{
}
