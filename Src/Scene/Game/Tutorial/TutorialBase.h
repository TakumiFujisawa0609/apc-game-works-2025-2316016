#pragma once
#include <functional>
#include <map>

class TutorialBase
{
public:

	enum class STATE
	{
		NONE,	//チュートリアル前
		PLAY,	//チュートリアル中
		END,	//チュートリアル後
	};

	TutorialBase(void);
	virtual ~TutorialBase(void) = default;
	virtual void Init(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	STATE GetState(void) const { return state_; }
	void ChangeState(STATE state);
protected:
	STATE state_;
	std::map<STATE, std::function<void(void)>> changeState_;	//状態変更時の処理
	virtual void ChangeStateNone(void);
	virtual void ChangeStatePlay(void);
	virtual void ChangeStateEnd(void);
	std::function<void(void)> updateState_;
	virtual void UpdateStateNone(void) = 0;
	virtual void UpdateStatePlay(void) = 0;
	virtual void UpdateStateEnd(void) = 0;
	std::function<void(void)> drawState_;
	virtual void DrawStateNone(void) = 0;
	virtual void DrawStatePlay(void) = 0;
	virtual void DrawStateEnd(void) = 0;
private:

};

