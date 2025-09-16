#include "Gravity.h"

Gravity::Gravity(void)
{
	InitChengeStateFunc();
}

Gravity::~Gravity(void)
{
}

void Gravity::Init(void)
{
}

void Gravity::Update(void)
{
	stateUpdate_();
}

void Gravity::Draw(void)
{
}

void Gravity::SetInitPower(float power)
{
	if (power != 0.0f)
	{
		return;
	}
	power_ = -abs(power);
}

void Gravity::ChengeState(STATE state)
{
	if (state_ != state)
	{
		state_ = state;
		stateChanges_[state_]();
	}
}

void Gravity::ChengeStateJump(void)
{
	stateUpdate_ = std::bind(&Gravity::UpdateStateJump, this);
}

void Gravity::ChengeStateChange(void)
{
	stateUpdate_ = std::bind(&Gravity::UpdateStateChange, this);
}

void Gravity::ChengeStateNone(void)
{
	stateUpdate_ = std::bind(&Gravity::UpdateStateNone, this);
	power_ = 0.0f;
}

void Gravity::UpdateStateJump(void)
{
	power_ += DECELERATION;
}

void Gravity::UpdateStateChange(void)
{

}

void Gravity::UpdateStateNone(void)
{
}

void Gravity::InitChengeStateFunc(void)
{
	stateChanges_[STATE::JUMP] = std::bind(&Gravity::ChengeStateJump, this);
	stateChanges_[STATE::CHANGE] = std::bind(&Gravity::ChengeStateChange, this);
	stateChanges_[STATE::NONE] = std::bind(&Gravity::ChengeStateNone, this);
}
