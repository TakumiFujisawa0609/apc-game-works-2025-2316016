#include "../../Utility/Utility.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/SoundManager.h"
#include "../../Manager/DrawTranslucentManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../EnemyBase.h"
#include "../../../Common/AnimationController.h"
#include "../SubObject/CrossLine.h"
#include "CrossAttack.h"

CrossAttack::CrossAttack(EnemyAttackManager& parent) : AttackBase(parent) ,radian_ (0.0f)
{
	range_ = RANGE::SHORT;
	geo_ = GEOMETORY::SPHERE;
	time_ = 0.0f;
	sumTime_ = 0.0f;
	reverseTime_ = 0.0f;
	myType_ = EnemyAttackManager::ATTACK_TYPE::CROSS_LINE;
	sign_ = 1;
	createInterval_ = 0.0f;
	createPointNum_ = 0;
	transform_ = std::make_shared<Transform>();
	material_ = std::make_unique<Polygon3DMaterial>(
		"FireVS.cso", 0,
		"FirePS.cso", 1
	);
	material_->AddConstBufPS({ time_, 0.0f, 1.0f, 1.0f });
	material_->AddTextureBuf(ResourceManager::GetInstance().Load(ResourceManager::SRC::WAVE_TEXTURE).handleId_);
	material_->AddTextureBuf(ResourceManager::GetInstance().Load(ResourceManager::SRC::NOISE).handleId_);
	renderer_ = std::make_shared<Polygon3DRenderer>(*material_, polygonInfo_);
	renderer_->SetBuckCull(true);
}
CrossAttack::~CrossAttack(void)
{
	SoundManager::GetInstance().Stop(SoundManager::SRC::FIRE);
}

void CrossAttack::Init(void)
{
}

void CrossAttack::Update(void)
{
	sumTime_ += SceneManager::GetInstance().GetDeltaTime();
	polygonInfo_.clear();
	updateState_();
	for (auto& line : crossLines_)
	{
		line->Update();
	}
	material_->SetConstBufPS(0, { sumTime_, 0.0f, 0.0f, 0.0f });
}

void CrossAttack::Draw(void)
{
	for (auto& line : crossLines_)
	{
		line->Draw();
	} 
	if (polygonInfo_.vertex.size() != 0)
	{
		DrawTranslucentManager::GetInstance().Add(transform_, renderer_);
	}
}

void CrossAttack::AddVertexs(Polygon3DRenderer::PolygonInfo info)
{
	int vertexSize = static_cast<int>(polygonInfo_.vertex.size());
	for (auto& vertex : info.vertex)
	{
		polygonInfo_.vertex.push_back(vertex);
	}
	for (auto& indices : info.Indices)
	{
		polygonInfo_.Indices.push_back(static_cast<unsigned short>(indices + vertexSize));
	}
}

void CrossAttack::ChangeStateNone(void)
{
	AttackBase::ChangeStateNone();
}

void CrossAttack::ChangeStateReady(void)
{
	AttackBase::ChangeStateReady();
	SoundManager::GetInstance().Play(SoundManager::SRC::FIRE, Sound::TIMES::LOOP);
	parent_.GetAnimController().Play(parent_.GetAnimNumber(EnemyAttackManager::ATTACK_STATE::READY, myType_));
}

void CrossAttack::ChangeStateStart(void)
{
	AttackBase::ChangeStateStart();
}

void CrossAttack::ChangeStateUpdate(void)
{
	AttackBase::ChangeStateUpdate();
	parent_.GetAnimController().Play(parent_.GetAnimNumber(EnemyAttackManager::ATTACK_STATE::PLAY, myType_));
}

void CrossAttack::ChangeStateFinish(void)
{
	deleyTime_ = COOL_DOWN;
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
			line = std::make_unique<CrossLine>(parent_.GetTransform().lock()->pos, radian_, Utility::Deg2RadF(360.0f / LINE_NUM * i), createPointNum_,*this);
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
