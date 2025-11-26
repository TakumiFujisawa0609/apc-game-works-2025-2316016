#include "../Common/AnimationController.h"
#include "TitlePlayer.h"

TitlePlayer::TitlePlayer(int playerNum) : PlayerBase(playerNum)
{
	ChangeState(STATE::IDLE, true);
}

TitlePlayer::~TitlePlayer(void)
{
}

void TitlePlayer::Init(void)
{
}

void TitlePlayer::Update(void)
{
	animCtrl_->Update();
}

void TitlePlayer::Draw(void)
{
	PlayerBase::Draw();
}

void TitlePlayer::UIDraw(void)
{
}

bool TitlePlayer::ChangeState(STATE state, bool isAbsolute)
{
	if (state_ != state || isAbsolute == true)
	{
		state_ = state;
		animCtrl_->Play(static_cast<int>(state_), true);
		return true;
	}
	return false;
}
