#include "../../Utility/Utility.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/SoundManager.h"
#include "../../EnemyBase.h"
#include "../../../Common/AnimationController.h"
#include "../SubObject/CrossLine.h"
#include "CrossAttack.h"

CrossAttack::CrossAttack(EnemyBase& enemy) : AttackBase(enemy)
{
	range_ = RANGE::SHORT;
	geo_ = GEOMETORY::SPHERE;
	time_ = 0.0f;
	radian_ = 0.0f;
	reverseTime_ = 0.0f;
	myType_ = EnemyBase::ATTACK_TYPE::CROSS_LINE;
	sign_ = 1;
	createInterval_ = 0.0f;
	createPointNum_ = 0;
}
CrossAttack::~CrossAttack(void)
{
}

void CrossAttack::Init(void)
{
}

void CrossAttack::Update(void)
{
	updateState_();
	for (auto& line : crossLines_)
	{
		line->Update();
	}
}

void CrossAttack::Draw(void)
{
	for (auto& line : crossLines_)
	{
		line->Draw();
	} 
}

Transform& CrossAttack::GetLineTransform(int lineNum)
{
	 return crossLines_[lineNum]->GetTransform();
}

void CrossAttack::ChangeStateNone(void)
{
	AttackBase::ChangeStateNone();
}

void CrossAttack::ChangeStateReady(void)
{
	AttackBase::ChangeStateReady();
	SoundManager::GetInstance().Play(SoundManager::SRC::FIRE, Sound::TIMES::LOOP);
	enemy_.GetAnimController().Play((int)EnemyBase::ANIM_TYPE_DRAGON::FLY_GLIDE);
}

void CrossAttack::ChangeStateStart(void)
{
	AttackBase::ChangeStateStart();
}

void CrossAttack::ChangeStateUpdate(void)
{
	AttackBase::ChangeStateUpdate();
	enemy_.GetAnimController().Play((int)EnemyBase::ANIM_TYPE_DRAGON::SCREAM);
}

void CrossAttack::ChangeStateFinish(void)
{
	deleyTime_ = COOL_DOWN;
	enemy_.GetAnimController().Play((int)EnemyBase::ANIM_TYPE_DRAGON::IDLE_1);
	SoundManager::GetInstance().Stop(SoundManager::SRC::FIRE);
	AttackBase::ChangeStateFinish();
}

void CrossAttack::UpdateStateNone(void)
{
}

void CrossAttack::UpdateStateReady(void)
{
	ChangeState(STATE::START);
}

void CrossAttack::UpdateStateStart(void)
{
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();
	createInterval_ -= deltaTime;
	if (createInterval_ < 0.0f)
	{
		for (int i = 0; i < LINE_NUM; i++)
		{
			std::unique_ptr<CrossLine> line;
			line = std::make_unique<CrossLine>(enemy_.GetTransform().pos, radian_, Utility::Deg2RadF(360.0f / LINE_NUM * i), createPointNum_);
			crossLines_.push_back(std::move(line));
		}
		createPointNum_++;
		createInterval_ = CREATE_INTERVAL;
	}
	if (createPointNum_ == LINE_POINT_NUM)
	{
		reverseTime_ = LINE_DIR_REVERSE_TIME;
		createPointNum_ = 0;
		ChangeState(STATE::UPDATE);
	}
}

void CrossAttack::UpdateStateUpdate(void)
{
	auto deltaTime = SceneManager::GetInstance().GetDeltaTime();
	time_ += deltaTime;
	reverseTime_ -= deltaTime;
	if (reverseTime_ < 0.0f)
	{
		sign_ *= -1;
		reverseTime_ = LINE_DIR_REVERSE_TIME;
	}
	radian_ += Utility::Deg2RadF(deltaTime * SECOND_TO_DEGREE * sign_);
	if (time_ > TIME)
	{
		crossLines_.clear();
		ChangeState(STATE::FINISH);
	}
}

void CrossAttack::UpdateStateFinish(void)
{
	deleyTime_ -= SceneManager::GetInstance().GetDeltaTime();
	if (deleyTime_ < 0.0f)
	{
		time_ = 0.0f;
		ChangeState(STATE::NONE);
	}
}
