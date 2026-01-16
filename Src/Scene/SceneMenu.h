#pragma once
#include "SceneBase.h"
#include "../../Manager/KeyConfig.h"

class SceneMenu :    public SceneBase
{
public:

	//余白
	static constexpr int MARGIN = 30;
	//選択肢の間隔
	static constexpr int INTERVAL_Y = 40;
	//選択肢の画像サイズ
	static constexpr int IMAGE_SIZE_Y = 64;
	//選択肢の描画サイズ
	static constexpr float DrawScale = 48.0f / static_cast<float>(IMAGE_SIZE_Y);
	//画像の描画位置
	static constexpr int START_POS_X = MARGIN + 100;
	static constexpr int START_POS_Y = MARGIN + 100;
	//選択中のオフセット
	static constexpr int SELECT_OFFSET_X = 40;
	//選択中の揺れ幅
	static constexpr float SELECT_SHAKE_WIDTH = 20.0f;
	static constexpr float SELECT_SHAKE_POWER = 5.0f;
	enum class TYPE
	{
		//KEY_CONFIG,	//キーコンフィグ
		CHACK_CONTROL, //操作確認
		OPTION,		//設定
		BACK_GAME,	//ゲームに戻る
		GO_TITLE,	//タイトルに戻る
		MAX,
	};

	SceneMenu(void);
	~SceneMenu(void)override;

	bool Init(void)override;
	void Update(void)override;
	void Draw(void)override;
private:

	//選択中のもの
	TYPE type_;
	//キーコンフィグのインスタンス
	KeyConfig& ins_;

	float time_;
};

