#include "../Common/AnimationController.h"
#include "OverPlayer.h"

OverPlayer::OverPlayer(int playerNum) : PlayerBase(playerNum)
{
    ChangeState(STATE::DEAD, true);
}

OverPlayer::~OverPlayer(void)
{
}

void OverPlayer::Init(void)
{
}

void OverPlayer::Update(void)
{
	animCtrl_->Update();
}

void OverPlayer::Draw(void)
{
	PlayerBase::Draw();
}

void OverPlayer::UIDraw(void)
{
}

bool OverPlayer::ChangeState(STATE state, bool isAbsolute)
{
	if (state_ != state || isAbsolute == true)
	{
		state_ = state;
		animCtrl_->Play(static_cast<int>(state_), false);
		return true;
	}
	return false;
}
