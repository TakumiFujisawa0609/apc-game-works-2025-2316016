#pragma once
#include <functional>
#include <map>
#include "SceneBase.h"
class SceneSetting :  public SceneBase
{
public:

	static constexpr float MOVE_SPEED = 0.001f;	//ゲージの動くスピード
	static constexpr int BOX_MARGIN = 50;	//背景の余白
	static constexpr int STRING_MARGIN = 30;	//文字の余白
	static constexpr int GAGE_WIDTH = 300;	//ゲージの幅
	static constexpr int GAGE_HEIGHT = 20;	//ゲージの高さ
	static constexpr int IMAGE_SIZE_X = 48;	//画像のサイズX
	static constexpr int IMAGE_SIZE_Y = IMAGE_SIZE_X / 2;	//画像のサイズY
	static constexpr int INTERVAL_Y = 40;	//選択肢の間隔

	enum class TYPE
	{
		BGM_VOLUME,		//BGMの音量調整
		SE_VOLUME,		//SEの音量調整
		FULL_SCREEN,	//フルスクリーンにするか
		END,		//設定を終わる
		MAX,
	};

	enum class STATE
	{
		CHOOSE,	//選ぶ状態
		OTHER,	//それぞれの処理を行う状態
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
	int frameCount_;
	void ChangeState(STATE state);
	std::map < STATE, std::function<void(void)>>stateChange_;
	void ChooseChange(void);
	void OtherChange(void);
	std::function<void(void)>stateUpdate_;
	void ChooseUpdate(void);
	void OtherUpdate(void);
};

