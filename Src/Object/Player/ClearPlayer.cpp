#include "../Common/AnimationController.h"
#include "ClearPlayer.h"

ClearPlayer::ClearPlayer(int playerNum) : PlayerBase(playerNum)
{
	ChangeState(STATE::IDLE, true);
}

ClearPlayer::~ClearPlayer(void)
{
}

void ClearPlayer::Init(void)
{
}

void ClearPlayer::Update(void)
{
	animCtrl_->Update();
}

void ClearPlayer::Draw(void)
{
	PlayerBase::Draw();
}

void ClearPlayer::UIDraw(void)
{
}

bool ClearPlayer::ChangeState(STATE state, bool isAbsolute)
{
	if (state_ != state || isAbsolute == true)
	{
		state_ = state;
		animCtrl_->Play(static_cast<int>(state_), true);
		return true;
	}
	return false;
}
