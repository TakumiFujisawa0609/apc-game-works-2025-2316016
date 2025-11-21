#include "TutorialBase.h"

TutorialBase::TutorialBase(void)
{
	changeState_[STATE::NONE] = std::bind(&TutorialBase::ChangeStateNone, this);
	changeState_[STATE::PLAY] = std::bind(&TutorialBase::ChangeStatePlay, this);
	changeState_[STATE::END] = std::bind(&TutorialBase::ChangeStateEnd, this);
	ChangeState(STATE::NONE);
}

void TutorialBase::ChangeState(STATE state)
{
	state_ = state;
	changeState_[state]();
}

void TutorialBase::ChangeStateNone(void)
{
	updateState_ = std::bind(&TutorialBase::UpdateStateNone, this);
	drawState_ = std::bind(&TutorialBase::DrawStateNone, this);
}

void TutorialBase::ChangeStatePlay(void)
{
	updateState_ = std::bind(&TutorialBase::UpdateStatePlay, this);
	drawState_ = std::bind(&TutorialBase::DrawStatePlay, this);
}

void TutorialBase::ChangeStateEnd(void)
{
	updateState_ = std::bind(&TutorialBase::UpdateStateEnd, this);
	drawState_ = std::bind(&TutorialBase::DrawStateEnd, this);
}