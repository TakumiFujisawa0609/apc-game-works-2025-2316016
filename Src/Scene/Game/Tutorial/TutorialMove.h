#pragma once
#include "TutorialBase.h"

class TutorialMove :  public TutorialBase
{
public:

	static constexpr float MOVE_DISTANCE = 300.0f; //‚Ç‚ê‚¾‚¯‚Ì‹——£“®‚¯‚ÎƒNƒŠƒA‚Æ‚·‚é‚©

	TutorialMove(void);
	~TutorialMove(void) override;
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void AddMoveDistance(float distance) { moveDistance_ += distance; }
private:
	float moveDistance_; //ˆÚ“®‚µ‚½‹——£

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

