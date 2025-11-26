#include "../../Utility/Utility.h"
#include "../../Manager/SceneManager.h"
#include "../../Renderer/ModelMaterial.h"
#include "../../Renderer/ModelRenderer.h"
#include "../../../Common/EffectController.h"
#include "../../../Common/Transform.h"
#include "../../../Common/AnimationController.h"
#include "../SubObject/WaterSprit.h"
#include "WaterSpritAttack.h"

WaterSpritAttack::WaterSpritAttack(EnemyBase& enemy) : AttackBase(enemy)
{
	range_ = RANGE::MIDDLE;
	geo_ = GEOMETORY::CIRCLE;
	myType_ = EnemyBase::ATTACK_TYPE::WATER_SPRIT;
	intervalTime_ = 0.0f;
}

WaterSpritAttack::~WaterSpritAttack()
{
}

void WaterSpritAttack::Init(void)
{
}

void WaterSpritAttack::Update(void)
{
	updateState_();
}

void WaterSpritAttack::Draw(void)
{
	for (auto& waterSprit : waterSprit_)
	{
		waterSprit->Draw();
	}
}

void WaterSpritAttack::ChangeStateNone(void)
{
	AttackBase::ChangeStateNone();
}

void WaterSpritAttack::ChangeStateReady(void)
{
	enemy_.GetAnimController().Play(enemy_.GetAnimNumber(EnemyBase::ATTACK_STATE::READY, myType_),false);
	AttackBase::ChangeStateReady();
}

void WaterSpritAttack::ChangeStateStart(void)
{
	CreateWaterSpritWave();
	intervalTime_ = INTERVAL_TIME;
	enemy_.GetAnimController().Play(enemy_.GetAnimNumber(EnemyBase::ATTACK_STATE::PLAY, myType_));
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
	if (enemy_.GetAnimController().IsEnd())
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
	VECTOR startPos = enemy_.GetTransform().lock()->pos;
	float randomDeg = static_cast<float>(GetRand(static_cast<int>(Utility::ONE_TRACK_DEG)));
	for (int i = 0; i < ONE_WATER_SPRIT_NUM; i++)
	{
		float rad = Utility::Deg2RadF(Utility::ONE_TRACK_DEG / ONE_WATER_SPRIT_NUM * i + randomDeg);
		VECTOR dir = { cosf(rad),0.0f,sinf(rad) };
		auto water = std::make_unique<WaterSprit>(dir, startPos, speed);
		waterSprit_.push_back(std::move(water));
	}
}
