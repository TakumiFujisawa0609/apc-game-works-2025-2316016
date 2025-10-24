#include "../../Utility/Utility.h"
#include "../../Manager/SceneManager.h"
#include "FallDownShot.h"
#include "FallDownAttack.h"

FallDownAttack::FallDownAttack(EnemyBase& enemy) : AttackBase(enemy)
{
	range_ = RANGE::ALL;
	geo_ = GEOMETORY::SPHERE;
	time_ = 0.0f;
	myType_ = EnemyBase::ATTACK_TYPE::FALL_DOWN;
}

FallDownAttack::~FallDownAttack(void)
{
}

void FallDownAttack::Init(void)
{
}

void FallDownAttack::Update(void)
{
	time_ -= SceneManager::GetInstance().GetDeltaTime();
	updateState_();
}

void FallDownAttack::Draw(void)
{
	for (auto& shot : fallDownShots_)
	{
		if (shot->GetState() == FallDownShot::STATE::DEAD)continue;
		shot->Draw();
	}

}

Transform& FallDownAttack::GetShotTransform(int shotNum)
{
	return fallDownShots_[shotNum]->GetTransform();
}

FallDownShot::STATE FallDownAttack::GetShotState(int shotNum)
{
	return fallDownShots_[shotNum]->GetState();
}

float FallDownAttack::GetShotRadius(int shotNum)
{
	return fallDownShots_[shotNum]->GetRadius();
}

void FallDownAttack::ChangeStateNone(void)
{
	AttackBase::ChangeStateNone();
}

void FallDownAttack::ChangeStateReady(void)
{
	AttackBase::ChangeStateReady();
}

void FallDownAttack::ChangeStateStart(void)
{
	time_ = TIME;
	//óéâ∫íeÇÃê∂ê¨
	for (int i = 0; i < MAX_FALL_NUM; i++)
	{
		std::unique_ptr<FallDownShot> fallDownShot = std::make_unique<FallDownShot>();
		fallDownShots_.push_back(std::move(fallDownShot));
	}
	AttackBase::ChangeStateStart();
}

void FallDownAttack::ChangeStateUpdate(void)
{
	AttackBase::ChangeStateUpdate();
}

void FallDownAttack::ChangeStateFinish(void)
{
	deleyTime_ = COOL_DOWN;
	AttackBase::ChangeStateFinish();
}

void FallDownAttack::UpdateStateNone(void)
{

}

void FallDownAttack::UpdateStateReady(void)
{
	ChangeState(STATE::START);
}

void FallDownAttack::UpdateStateStart(void)
{
	ChangeState(STATE::UPDATE);
}

void FallDownAttack::UpdateStateUpdate(void)
{
	//óéâ∫íeÇÃçXêV
	for (auto& shot : fallDownShots_)
	{
		shot->Update();
		if (shot->GetState() == FallDownShot::STATE::DEAD)
		{
			shot = nullptr;
		}
	}
	//nullptrÇÃèÍèäÇäJï˙Ç∑ÇÈ
	Utility::EraseVectorAllay(fallDownShots_);
	//óéâ∫íeÇÃê∂ê¨
	if (time_ > 0.0f)
	{
		int aliveNum = GetFallDownShotNum();
		int createNum = MAX_FALL_NUM - aliveNum;
		for (int i = 0; i < createNum; i++)
		{
			std::unique_ptr<FallDownShot> fallDownShot = std::make_unique<FallDownShot>();
			fallDownShots_.push_back(std::move(fallDownShot));
		}
	}
	int aliveNum = GetFallDownShotNum();
	if (aliveNum == 0)
	{
		ChangeState(STATE::FINISH);
	}

}

void FallDownAttack::UpdateStateFinish(void)
{
	deleyTime_ -= SceneManager::GetInstance().GetDeltaTime();
	if (deleyTime_ < 0.0f)
	{
		ChangeState(STATE::NONE);
	}
}
