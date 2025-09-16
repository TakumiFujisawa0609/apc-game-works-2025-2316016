#include "../Common/Gravity.h"
#include "PlayerBase.h"

PlayerBase::PlayerBase(void)
{
	hp_ = MAX_HP;
	transform_ = std::make_unique<Transform>();
	gravity_ = std::make_unique<Gravity>();
}

PlayerBase::~PlayerBase(void)
{
}

void PlayerBase::Init(void)
{
}

void PlayerBase::Update(void)
{
}

void PlayerBase::Draw(void)
{
}

void PlayerBase::ChengeGravityDir(void)
{
}
