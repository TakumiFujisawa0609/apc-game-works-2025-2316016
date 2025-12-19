#include "../../Utility/Utility.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/DrawTranslucentManager.h"
#include "../../Renderer/ModelMaterial.h"
#include "../../Renderer/ModelRenderer.h"
#include "../../../Common/EffectController.h"
#include "../../../Common/Transform.h"
#include "../../../Common/AnimationController.h"
#include "../SubObject/WaterSprit.h"
#include "WaterSpritAttack.h"

WaterSpritAttack::WaterSpritAttack(EnemyAttackManager& parent) : AttackBase(parent)
{
	range_ = RANGE::MIDDLE;
	geo_ = GEOMETORY::CIRCLE;
	myType_ = EnemyAttackManager::ATTACK_TYPE::WATER_SPRIT;
	intervalTime_ = 0.0f;
	transform_ = std::make_shared<Transform>();
	material_ = std::make_unique<Polygon3DMaterial>(
		"WaterSpritVS.cso", 1,
		"WaterSpritPS.cso", 1
	);
	material_->AddConstBufVS({ TEXTURE_SCALE_X, TEXTURE_SCALE_Y, 1.0f, 1.0f });
	material_->AddConstBufPS({ sumTime_, TIME_SCALE, 1.0f, 1.0f });
	material_->AddTextureBuf(ResourceManager::GetInstance().Load(ResourceManager::SRC::TEXTURE_1).handleId_);
	//material_->AddTextureBuf(ResourceManager::GetInstance().Load(ResourceManager::SRC::NOISE).handleId_);
	renderer_ = std::make_shared<Polygon3DRenderer>(*material_, polygonInfo_);
	renderer_->SetBuckCull(true);
}

WaterSpritAttack::~WaterSpritAttack()
{
}

void WaterSpritAttack::Init(void)
{
}

void WaterSpritAttack::Update(void)
{
	polygonInfo_.clear();
	sumTime_ += SceneManager::GetInstance().GetDeltaTime();
	updateState_();
	material_->SetConstBufPS(0,{ sumTime_, TIME_SCALE, 1.0f, 1.0f });
}

void WaterSpritAttack::Draw(void)
{
	for (auto& waterSprit : waterSprit_)
	{
		waterSprit->Draw();
	}
	if (polygonInfo_.vertex.size() == 0)
	{
		return;
	}
	DrawTranslucentManager::GetInstance().Add(transform_, renderer_);
}

void WaterSpritAttack::ChangeStateNone(void)
{
	AttackBase::ChangeStateNone();
}

void WaterSpritAttack::ChangeStateReady(void)
{
	parent_.GetAnimController().Play(parent_.GetAnimNumber(EnemyAttackManager::ATTACK_STATE::READY, myType_),false);
	AttackBase::ChangeStateReady();
}

void WaterSpritAttack::ChangeStateStart(void)
{
	CreateWaterSpritWave();
	intervalTime_ = INTERVAL_TIME;
	parent_.GetAnimController().Play(parent_.GetAnimNumber(EnemyAttackManager::ATTACK_STATE::PLAY, myType_));
	AttackBase::ChangeStateStart();
}

void WaterSpritAttack::ChangeStateUpdate(void)
{
	AttackBase::ChangeStateUpdate();
}

void WaterSpritAttack::ChangeStateFinish(void)
{
	AttackBase::ChangeStateFinish();
	deleyTime_ = COOL_DOWN;
	waveNum_ = 0;
}

void WaterSpritAttack::UpdateStateNone(void)
{
}

void WaterSpritAttack::UpdateStateReady(void)
{
	if (parent_.GetAnimController().IsEnd())
	{
		ChangeState(STATE::START);
	}
}

void WaterSpritAttack::UpdateStateStart(void)
{
	ChangeState(STATE::UPDATE);
}

void WaterSpritAttack::UpdateStateUpdate(void)
{
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();
	intervalTime_ -= deltaTime;
	if (intervalTime_ < 0.0f && waveNum_ < MAX_WATER_SPRIT_WAVE_NUM)
	{
		CreateWaterSpritWave();
		intervalTime_ = INTERVAL_TIME;
	}
	for (auto& waterSprit : waterSprit_)
	{
		waterSprit->Update();
		if (waterSprit->IsEnd())
		{
			waterSprit = nullptr;
		}
	}
	Utility::EraseVectorAllay(waterSprit_);
	if( GetSubObjectNum() == 0)
	{
		ChangeState(STATE::FINISH);
	}
}

void WaterSpritAttack::UpdateStateFinish(void)
{
	deleyTime_ -= SceneManager::GetInstance().GetDeltaTime();
	if (deleyTime_ < 0.0f)
	{
		ChangeState(STATE::NONE);
	}
}

void WaterSpritAttack::CreateWaterSpritWave(void)
{
	waveNum_++;
	float speed = GetRand(static_cast<int>(MAX_SPEED - MIN_SPEED)) + MIN_SPEED;
	VECTOR startPos = parent_.GetTransform().lock()->pos;
	float randomDeg = static_cast<float>(GetRand(static_cast<int>(Utility::ONE_TRACK_DEG)));
	for (int i = 0; i < ONE_WATER_SPRIT_NUM; i++)
	{
		float rad = Utility::Deg2RadF(Utility::ONE_TRACK_DEG / ONE_WATER_SPRIT_NUM * i + randomDeg);
		VECTOR dir = { cosf(rad),0.0f,sinf(rad) };
		auto water = std::make_unique<WaterSprit>(dir, startPos, speed,*this);
		waterSprit_.push_back(std::move(water));
	}
}