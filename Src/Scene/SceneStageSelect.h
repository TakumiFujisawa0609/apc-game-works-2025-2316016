#pragma once
#include <DxLib.h>
#include <vector>
#include <map>
#include <functional>
#include "../../Manager/KeyConfig.h"
#include "SceneBase.h"

class SceneStageSelect : public SceneBase
{
public:

	enum class STATE
	{
		CHOOSE,	//選択中
		CHECK,	//決定確認
		DECIDE,	//決定
	};

	//メンバー関数
	SceneStageSelect(void);

	~SceneStageSelect(void) override;

	//初期化処理(初回の1度のみ実行される)
	bool Init(void)override;

	//更新処理
	void Update(void) override;

	//描画処理
	void Draw(void) override;

private:

	void DrawEnterCancelKey(void);
	void DrawEnterCancelKeyTemp(void);
};