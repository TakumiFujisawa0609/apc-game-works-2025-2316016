#pragma once
#include <DxLib.h>
#include <vector>
#include <map>
#include <functional>
#include "../../Manager/KeyConfig.h"
#include "SceneBase.h"

class Control;

class SceneSelect : public SceneBase
{
public:

	enum class SELECT
	{
		CONTROL_TYPE,	//操作方法
		STAGE_SELECT,	//ステージ選択
	};

	//メンバー関数
	SceneSelect(void);

	~SceneSelect(void) override;

	//初期化処理(初回の1度のみ実行される)
	bool Init(void)override;

	//更新処理
	void Update(void) override;

	//描画処理
	void Draw(void) override;

private:

	std::unique_ptr<Control> control_; //操作方法選択用
	SELECT select_; //選択している項目
	std::map<SELECT, std::function<void(void)>> changeSelect_; //選択項目変更時の関数格納用
	void ChangeSelect(SELECT select);
	void ChangeSelectControlType(void);
	void ChangeSelectStageSelect(void);
	std::function<void(void)> updateSelect_; //選択項目更新時の関数格納用
	void UpdateSelectControlType(void);
	void UpdateSelectStageSelect(void);
	std::function<void(void)> drawSelect_; //選択項目描画時の関数格納用
	void DrawSelectControlType(void);
	void DrawSelectStageSelect(void);

	void DrawEnterCancelKey(void);
	void DrawEnterCancelKeyTemp(void);
};