#pragma once
#include <functional>
#include "../../Common/Vector2.h"

class PadUI
{
public:
	static constexpr int PAD_IMAGE_SIZE_X = 465;
	static constexpr int PAD_IMAGE_SIZE_Y = 256;
	static constexpr float CHANGE_TIME = 0.5f;

	enum class STATE
	{
		SHOW,
		REDUCATE,	//è¨Ç≥Ç≠Ç∑ÇÈ
		HIDE,
		ENLARGE,	//ëÂÇ´Ç≠Ç∑ÇÈ
	};

	PadUI(void);
	~PadUI(void);
	void Init(void);
	void Update(void);
	void Draw(void);
private:
	STATE state_;
	float time_;
	Vector2 size_;
	FLOAT2 changeVec_;//ÇPïbÇÃïœâªó 
	void ChangeState(STATE newState);
	std::map < STATE, std::function<void(void)>> changeState_;
	void ChangeToShow(void);
	void ChangeToHide(void);
	void ChangeToEnlarge(void);
	void ChangeToReducte(void);
	std::function<void(void)> stateUpdate_;
	void ShowUpdate(void);
	void HideUpdate(void);
	void EnlargeUpdate(void);
	void ReducteUpdate(void);
};