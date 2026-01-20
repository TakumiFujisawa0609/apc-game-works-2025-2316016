#pragma once
#include <functional>
#include <map>
#include "SceneBase.h"
#include "../Manager/DataBank.h"

class SceneSetting :  public SceneBase
{
public:

	static constexpr float MOVE_SPEED = 0.001f;	//ゲージの動くスピード
	static constexpr int BOX_MARGIN = 50;	//背景の余白
	static constexpr int STRING_MARGIN = 30;	//文字の余白
	static constexpr int GAGE_WIDTH = 300;	//ゲージの幅
	static constexpr int IMAGE_SIZE_Y = 64;	//画像のサイズY
	static constexpr int INTERVAL_Y = 10;	//選択肢の間隔

	enum class TYPE
	{
		BGM_VOLUME,		//BGMの音量調整
		SE_VOLUME,		//SEの音量調整
		SCREEN_MODE,	//フルスクリーンにするか
		LOCKON_MODE,	//ロックオンモードの切り替え
		END,		//設定を終わる
		MAX,
	};

	enum class STATE
	{
		CHOOSE,	//選ぶ状態
		OTHER,	//それぞれの処理を行う状態
	};

	enum class SCREEN_MODE_TYPE
	{
		FULL_SCREEN,	//フルスクリーン
		WINDOW_SCREEN,	//ウィンドウ
		MAX
	};

	SceneSetting(void);
	~SceneSetting(void)override;

	bool Init(void)override;
	void Update(void)override;
	void Draw(void)override;
private:
	TYPE type_;
	STATE state_;
	float seVolume_;
	float bgmVolume_;
	SCREEN_MODE_TYPE screenType_;	//現在のスクリーンモードの種類
	SCREEN_MODE_TYPE selectScreenType_;	//選択中のスクリーンモードの種類
	DataBank::LOCKON_MODE lockonMode_;	//現在のロックオンモード
	DataBank::LOCKON_MODE selectLockonMode_;	//選択中のロックオンモード
	int frameCount_;
	void ChangeState(STATE state);
	std::map < STATE, std::function<void(void)>>stateChange_;
	void ChooseChange(void);
	void OtherChange(void);
	std::function<void(void)>stateUpdate_;
	void ChooseUpdate(void);
	void OtherUpdate(void);

	void DrawBackBox(void);
	int GetLockOnModeHandle(DataBank::LOCKON_MODE mode);
};

