#pragma once
#include "TutorialBase.h"
class TutorialLockOn :  public TutorialBase
{
public:
	TutorialLockOn(void);
	virtual ~TutorialLockOn(void) = default;
	virtual void Init(void)override;
	virtual void Update(void)override;
	virtual void Draw(void)override;
protected:
	
	void ChangeStateNone(void) override;
	void ChangeStatePlay(void) override;
	void ChangeStateEnd(void) override;
	void UpdateStateNone(void) override;
	void UpdateStatePlay(void) override;
	void UpdateStateEnd(void) override;
	void DrawStateNone(void) override;
	void DrawStatePlay(void) override;
	void DrawStateEnd(void) override;

};

