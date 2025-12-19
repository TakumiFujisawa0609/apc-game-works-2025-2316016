#include <cmath>
#include "../../Utility/Utility.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SoundManager.h"
#include "../../Manager/DrawTranslucentManager.h"
#include "../../../Common/AnimationController.h"
#include "../../EnemyBase.h"
#include "../SubObject/ThunderAround.h"
#include "ThunderAroundAttack.h"

ThunderAroundAttack::ThunderAroundAttack(EnemyAttackManager& parent) : AttackBase(parent)
{
	range_ = RANGE::ALL;
	geo_ = GEOMETORY::CIRCLE;
	time_ = 0.0f;
	sumTime_ = 0.0f;
	intervalTime_ = 0.0f;
	myType_ = EnemyAttackManager::ATTACK_TYPE::THUNDER_AROUND;
	transform_ = std::make_shared<Transform>();
	material_ = std::make_unique<Polygon3DMaterial>(
		"ThunderVS.cso", 1,
		"ThunderPS.cso", 1
	);
	material_->AddConstBufVS({ TEXTURE_SCALE_X, TEXTURE_SCALE_Y, 1.0f, 1.0f });
	material_->AddConstBufPS({ sumTime_, TIME_SCALE, THRESHOLD, 1.0f });
	material_->AddTextureBuf(ResourceManager::GetInstance().Load(ResourceManager::SRC::TEXTURE_1).handleId_);
	//material_->AddTextureBuf(ResourceManager::GetInstance().Load(ResourceManager::SRC::NOISE).handleId_);
	renderer_ = std::make_shared<Polygon3DRenderer>(*material_, polygonInfo_);
	renderer_->SetBuckCull(true);
}

ThunderAroundAttack::~ThunderAroundAttack(void)
{
}

void ThunderAroundAttack::Init(void)
{
}

void ThunderAroundAttack::Update(void)
{
	sumTime_ += SceneManager::GetInstance().GetDeltaTime();
	polygonInfo_.clear();
	updateState_();
	material_->SetConstBufPS(0, { sumTime_, TIME_SCALE, THRESHOLD, 1.0f });
}

void ThunderAroundAttack::Draw(void)
{
	for (auto& thunder : thunders_)
	{
		if (thunder->IsDead())continue;
		thunder->Draw();
	}
	if (polygonInfo_.vertex.size() == 0)
	{
		return;
	}
	DrawTranslucentManager::GetInstance().Add(transform_, renderer_);
}

void ThunderAroundAttack::ChangeStateNone(void)
{
	AttackBase::ChangeStateNone();
}

void ThunderAroundAttack::ChangeStateReady(void)
{
	parent_.GetAnimController().Play(parent_.GetAnimNumber(EnemyAttackManager::ATTACK_STATE::READY, myType_),false);
	AttackBase::ChangeStateReady();
}

void ThunderAroundAttack::ChangeStateStart(void)
{
	parent_.GetAnimController().Play(parent_.GetAnimNumber(EnemyAttackManager::ATTACK_STATE::PLAY, myType_));
	CreateThunder();
	time_ = TIME;
	intervalTime_ = INTERVAL_TIME;
	SoundManager::GetInstance().Play(SoundManager::SRC::THUNDER, Sound::TIMES::LOOP);
	AttackBase::ChangeStateStart();
}

void ThunderAroundAttack::ChangeStateUpdate(void)
{
	AttackBase::ChangeStateUpdate();
}

void ThunderAroundAttack::ChangeStateFinish(void)
{
	AttackBase::ChangeStateFinish();
	SoundManager::GetInstance().Stop(SoundManager::SRC::THUNDER);
	deleyTime_ = COOL_DOWN;
}

void ThunderAroundAttack::UpdateStateNone(void)
{
}

void ThunderAroundAttack::UpdateStateReady(void)
{
	if (parent_.GetAnimController().IsEnd())
	{
		ChangeState(STATE::START);
	}
}

void ThunderAroundAttack::UpdateStateStart(void)
{

	ChangeState(STATE::UPDATE);
}

void ThunderAroundAttack::UpdateStateUpdate(void)
{
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();	
	time_ -= deltaTime;
	intervalTime_ -= deltaTime;
	if (intervalTime_ < 0.0f && time_ > 0.0f )
	{
		CreateThunder();
		intervalTime_ = INTERVAL_TIME;
	}
	
	bool isFinish = true;
	for (auto& thunder : thunders_)
	{
		if (!thunder->IsDead())
		{
			isFinish = false;
			thunder->Update();
		}
		else
		{
			thunder = nullptr;
		}
	}
	Utility::EraseVectorAllay(thunders_);
	if (isFinish)
	{
		ChangeState(STATE::FINISH);
	}
}

void ThunderAroundAttack::UpdateStateFinish(void)
{
	deleyTime_ -= SceneManager::GetInstance().GetDeltaTime();
	if (deleyTime_ < 0.0f)
	{
		ChangeState(STATE::NONE);
	}
}

void ThunderAroundAttack::CreateThunder(void)
{
	float rad = 0.0f;
	//ƒTƒ“ƒ_[‚Ì¶¬
	for (int i = 0; i < THUNDER_NUM; i++)
	{
		rad = Utility::Deg2RadF((360.0f / THUNDER_NUM) * i);
		std::unique_ptr<ThunderAround> thunder;
		VECTOR targetPos = target_.lock()->pos;
		VECTOR initPos = VAdd(targetPos, VGet(cosf(rad) * DISTANCE, 0.0f, sinf(rad) * DISTANCE));
		thunder = std::make_unique<ThunderAround>(targetPos, initPos, rad,*this);
		thunders_.push_back(std::move(thunder));
	}
}