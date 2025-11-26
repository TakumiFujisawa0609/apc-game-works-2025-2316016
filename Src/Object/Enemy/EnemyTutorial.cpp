#include "../../Utility/Utility.h"
#include "../../Manager/ResourceManager.h"
#include "../../Renderer/ModelMaterial.h"
#include "../../Renderer/ModelRenderer.h"
#include "../Common/EffectController.h"
#include "../Common/Transform.h"
#include "../Common/Gravity.h"
#include "../Common/AnimationController.h"
#include "EnemyTutorial.h"

EnemyTutorial::EnemyTutorial(std::weak_ptr<Transform> target) : EnemyBase(target)
{
	//damageTime_ = 0.0f;
	//hitPos_ = Utility::VECTOR_ZERO;
	//transform_ = std::make_shared<Transform>();

	//material_ = std::make_unique<ModelMaterial>(
	//	"EnemyVS.cso", 0,
	//	"EnemyPS.cso", 4
	//);
	////material_->AddConstBufVS({ TEXTURE_SCALE, 0.0f, 1.0f, 1.0f });
	//material_->AddConstBufPS(static_cast<FLOAT4>(Utility::COLOR_F2FLOAT4(DEFAULT_COLOR)));
	//material_->AddConstBufPS(static_cast<FLOAT4>(Utility::COLOR_F2FLOAT4(DAMAGE_COLOR_ADD)));
	//material_->AddConstBufPS({ damageTime_, hitPos_.x, hitPos_.y, hitPos_.z });
	//material_->AddConstBufPS({ DAMAGE_EFECT_RADIUS,0.0f, 0.0f, 0.0f });
	////material_->SetTextureBuf(3, ResourceManager::GetInstance().Load(ResourceManager::SRC::NOISE).handleId_);
	//renderer_ = std::make_shared<ModelRenderer>(
	//	transform_->modelId, *material_
	//);
	//gravity_ = std::make_unique<Gravity>();
	//gravity_->Init();
	//gravity_->SetDir(Utility::DIR_D);
	//maxHP_ = 100.0f;
	//hp_ = maxHP_;
	//AplayChangeStateFunc();
	//InitAddAttack();
	//ChangeState(STATE::IDLE);
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
