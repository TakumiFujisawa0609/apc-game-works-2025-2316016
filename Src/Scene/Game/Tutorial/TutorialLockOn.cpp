#include "TutorialLockOn.h"

TutorialLockOn::TutorialLockOn(void)
{
}

void TutorialLockOn::Init(void)
{
}

void TutorialLockOn::Update(void)
{
	updateState_();
}

void TutorialLockOn::Draw(void)
{
}

void TutorialLockOn::ChangeStateNone(void)
{
	TutorialBase::ChangeStateNone();
}

void TutorialLockOn::ChangeStatePlay(void)
{
	TutorialBase::ChangeStatePlay();
}

void TutorialLockOn::ChangeStateEnd(void)
{
	TutorialBase::ChangeStateEnd();
}

void TutorialLockOn::UpdateStateNone(void)
{
	return;
}

void TutorialLockOn::UpdateStatePlay(void)
{
}

void TutorialLockOn::UpdateStateEnd(void)
{
	return;
}

void TutorialLockOn::DrawStateNone(void)
{
	return;
}

void TutorialLockOn::DrawStatePlay(void)
{
}

void TutorialLockOn::DrawStateEnd(void)
{
	return;
}
