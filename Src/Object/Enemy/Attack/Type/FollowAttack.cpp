#include "../../Utility/Utility.h"
#include "../../../Common/Transform.h"
#include "../../../Common/AnimationController.h"
#include "../../Manager/Camera.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/DrawTranslucentManager.h"
#include "../../EnemyBase.h"
#include "../SubObject/FollowShot.h"
#include "FollowAttack.h"

FollowAttack::FollowAttack(EnemyAttackManager& parent) : AttackBase(parent)
{
	time_ = 0.0f;
	range_ = RANGE::LONG;
	geo_ = GEOMETORY::SPHERE;
	myType_ = EnemyAttackManager::ATTACK_TYPE::FOLLOW;
	transform_ = std::make_shared<Transform>();
	material_ = std::make_unique<Polygon3DMaterial>(
		"FollowVS.cso", 0,
		"FollowPS.cso", 1
	);
	VECTOR cameraPos = GetCameraPosition();
	material_->AddConstBufPS({ time_, cameraPos.x, cameraPos.y, cameraPos.z });
	material_->AddTextureBuf(ResourceManager::GetInstance().Load(ResourceManager::SRC::TEXTURE_1).handleId_);
	material_->AddTextureBuf(ResourceManager::GetInstance().Load(ResourceManager::SRC::NOISE).handleId_);
	renderer_ = std::make_shared<Polygon3DRenderer>(*material_, polygonInfo_);
	//renderer_->SetBuckCull(true);
}

FollowAttack::~FollowAttack(void)
{
}

void FollowAttack::Init(void)
{
}

void FollowAttack::Update(void)
{
	time_ += SceneManager::GetInstance().GetDeltaTime();
	updateState_();
}

void FollowAttack::Draw(void)
{
	VECTOR cameraPos = SceneManager::GetInstance().GetCamera().GetPos();
	material_->SetConstBufPS(0, { time_, cameraPos.x, cameraPos.y, cameraPos.z });
	polygonInfo_.clear();
	for (auto& shot : shots_)
	{
		if (!shot->IsShot())continue;
		shot->Draw();
	}
	if (polygonInfo_.vertex.size() != 0)
	{
		DrawTranslucentManager::GetInstance().Add(transform_, renderer_);
	}
}

void FollowAttack::ChangeStateNone(void)
{
	AttackBase::ChangeStateNone();
}

void FollowAttack::ChangeStateReady(void)
{
	parent_.GetAnimController().Play(parent_.GetAnimNumber(EnemyAttackManager::ATTACK_STATE::READY, myType_),false);
	AttackBase::ChangeStateReady();
}

void FollowAttack::ChangeStateStart(void)
{
	parent_.GetAnimController().Play(parent_.GetAnimNumber(EnemyAttackManager::ATTACK_STATE::PLAY, myType_));
	AttackBase::ChangeStateStart();
	std::unique_ptr<FollowShot> slow = std::make_unique<FollowShot>(target_,FollowShot::SPEED_TYPE::SLOW,parent_.GetTransform().lock()->pos,*this);
	std::unique_ptr<FollowShot> midium = std::make_unique<FollowShot>(target_, FollowShot::SPEED_TYPE::MIDIUM, parent_.GetTransform().lock()->pos, *this);
	std::unique_ptr<FollowShot> fast = std::make_unique<FollowShot>(target_, FollowShot::SPEED_TYPE::FAST, parent_.GetTransform().lock()->pos,*this);
	shots_.push_back(std::move(slow));
	shots_.push_back(std::move(midium));
	shots_.push_back(std::move(fast));
	for (int i = 0; i < RANDOM_SHOT_NUM; i++)
	{
		std::unique_ptr<FollowShot> random = std::make_unique<FollowShot>(target_, FollowShot::SPEED_TYPE::RANDOM, parent_.GetTransform().lock()->pos, *this);
		shots_.push_back(std::move(random));
	}
}

void FollowAttack::ChangeStateUpdate(void)
{
	AttackBase::ChangeStateUpdate();
}

void FollowAttack::ChangeStateFinish(void)
{
	AttackBase::ChangeStateFinish();
	deleyTime_ = COOL_DOWN;
}

void FollowAttack::UpdateStateNone(void)
{
}

void FollowAttack::UpdateStateReady(void)
{
	if (parent_.GetAnimController().IsEnd())
	{
		ChangeState(STATE::START);
	}
}

void FollowAttack::UpdateStateStart(void)
{
	ChangeState(STATE::UPDATE);
}

void FollowAttack::UpdateStateUpdate(void)
{
	time_ += SceneManager::GetInstance().GetDeltaTime();
	bool isFinish = true;
	for (auto& shot : shots_)
	{
		if (shot->IsShot())
		{
			isFinish = false;
			shot->Update();
		}
		else 
		{
			shot = nullptr;
		}
	}
	Utility::EraseVectorAllay(shots_); 
	if (isFinish)
	{
		ChangeState(STATE::FINISH);
	}
	material_->SetConstBufPS(0, { time_, NOISE_POW, 1.0f, 1.0f });
}

void FollowAttack::UpdateStateFinish(void)
{
	deleyTime_ -= SceneManager::GetInstance().GetDeltaTime();
	if (deleyTime_ < 0.0f)
	{
		ChangeState(STATE::NONE);
	}
}
