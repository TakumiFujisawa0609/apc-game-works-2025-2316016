#pragma once
#include "SceneBase.h"
#include "../../Manager/KeyConfig.h"

class SceneMenu :    public SceneBase
{
public:

	//余白
	static constexpr int MARGIN = 30;

	enum class TYPE
	{
		KEY_CONFIG,	//キーコンフィグ
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
};

