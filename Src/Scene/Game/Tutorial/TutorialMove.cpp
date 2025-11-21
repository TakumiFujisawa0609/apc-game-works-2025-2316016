#include "TutorialMove.h"

TutorialMove::TutorialMove(void)
{
	moveDistance_ = 0.0f;
}

TutorialMove::~TutorialMove(void)
{
}

void TutorialMove::Init(void)
{
}

void TutorialMove::Update(void)
{
	updateState_();
}

void TutorialMove::Draw(void)
{
}

void TutorialMove::ChangeStateNone(void)
{
	TutorialBase::ChangeStateNone();
}

void TutorialMove::ChangeStatePlay(void)
{
	moveDistance_ = 0.0f;
	TutorialBase::ChangeStatePlay();
}

void TutorialMove::ChangeStateEnd(void)
{
	TutorialBase::ChangeStateEnd();
}

void TutorialMove::UpdateStateNone(void)
{
	return;
}

void TutorialMove::UpdateStatePlay(void)
{
	if (moveDistance_ >= MOVE_DISTANCE)
	{
		ChangeState(STATE::END);
		return;
	}
}

void TutorialMove::UpdateStateEnd(void)
{
	return;
}

void TutorialMove::DrawStateNone(void)
{
	return;
}

void TutorialMove::DrawStatePlay(void)
{

}

void TutorialMove::DrawStateEnd(void)
{
	return;
}
