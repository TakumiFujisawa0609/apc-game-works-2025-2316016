#include "../../Utility/Utility.h"
#include "../../Manager/SceneManager.h"
#include "../../Renderer/ModelMaterial.h"
#include "../../Renderer/ModelRenderer.h"
#include "../../../Common/EffectController.h"
#include "ThunderAround.h"

ThunderAround::ThunderAround(VECTOR targetPos, VECTOR initPos,  float initRad) : initRad_(initRad)
{
	targetPos_ = targetPos;
	targetPos_.y = 0.0f;
	transform_ = std::make_unique<Transform>();
	transform_->SetModel(-1);
	transform_->pos = initPos;
	transform_->pos.y = 0.0f;
	transform_->Update();
	rad_ = 0.0f;
	dir_ = VNorm(VSub(targetPos, initPos));
	distance_ = Utility::Distance(targetPos,initPos);
	distance_ += MOVE_ADD_DISTANCE;
	changeState_[STATE::REDUCTION] = std::bind(&ThunderAround::ChangeStateReduction, this);
	changeState_[STATE::ROTATION] = std::bind(&ThunderAround::ChangeStateRotation, this);
	changeState_[STATE::EXPANSION] = std::bind(&ThunderAround::ChangeStateExpansion, this);
	changeState_[STATE::DEAD] = std::bind(&ThunderAround::ChangeStateDead, this);
	ChangeState(STATE::REDUCTION);
}

ThunderAround::~ThunderAround(void)
{
}

void ThunderAround::Init(void)
{
}

void ThunderAround::Update(void)
{
	time_ -= SceneManager::GetInstance().GetDeltaTime();
	updateState_();
}

void ThunderAround::Draw(void)
{
	Utility::DrawCircle3DXZ(transform_->pos, RADIUS, VERTEX_NUM, GetColor(255, 0, 0), true);
}

void ThunderAround::ChangeState(STATE state)
{
	state_ = state;
	changeState_[state]();
}

void ThunderAround::ChangeStateReduction(void)
{
	time_ = REDUCTION_TIME;
	updateState_ = std::bind(&ThunderAround::UpdateStateReduction, this);
}

void ThunderAround::ChangeStateRotation(void)
{
	time_ = ROTATION_TIME;
	updateState_ = std::bind(&ThunderAround::UpdateStateRotation, this);
}

void ThunderAround::ChangeStateExpansion(void)
{
	time_ = EXPANSION_TIME;
	dir_ = VNorm(VSub(transform_->pos, targetPos_));
	updateState_ = std::bind(&ThunderAround::UpdateStateExpansion, this);
}

void ThunderAround::ChangeStateDead(void)
{
	updateState_ = std::bind(&ThunderAround::UpdateStateDead, this);
}

void ThunderAround::UpdateStateReduction(void)
{
	auto deltaTime = SceneManager::GetInstance().GetDeltaTime();
	transform_->pos = VAdd(transform_->pos, VScale(dir_, distance_ / (REDUCTION_TIME / deltaTime)));
	if (time_ < 0.0f)
	{
		ChangeState(STATE::ROTATION);
	}
}

void ThunderAround::UpdateStateRotation(void)
{
	rad_ += Utility::Deg2RadF(ROTATION_SPEED  * SceneManager::GetInstance().GetDeltaTime());
	auto distance = Utility::Distance(targetPos_, transform_->pos);
	transform_->pos.x = targetPos_.x + distance * cosf(rad_ + initRad_);
	transform_->pos.y = targetPos_.y;
	transform_->pos.z = targetPos_.z + distance * sinf(rad_ + initRad_);
	transform_->Update();
	if (time_ < 0.0f)
	{
		ChangeState(STATE::EXPANSION);
	}
}

void ThunderAround::UpdateStateExpansion(void)
{
	auto deltaTime = SceneManager::GetInstance().GetDeltaTime();
	transform_->pos = VAdd(transform_->pos, VScale(dir_, distance_ / (EXPANSION_TIME / deltaTime)));
	if (time_ < 0.0f)
	{
		ChangeState(STATE::DEAD);
	}
}

void ThunderAround::UpdateStateDead(void)
{
}
