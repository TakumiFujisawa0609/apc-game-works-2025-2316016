#pragma once
#include <DxLib.h>
#include <vector>
#include <map>
#include <functional>
#include "../../Manager/KeyConfig.h"
#include "SceneBase.h"

class SceneSelect : public SceneBase
{
public:

	enum class SELECT
	{
		CONTROL_TYPE,	//操作方法
		STAGE_SELECT,	//ステージ選択
	};

	enum class CONTROL_TYPE
	{
		ENTER,	//決定
		AVOID,	//回避
		JUMP,	//ジャンプ
		ATTACK,	//攻撃
		ROCK_ON,	//ロックオン
		MAX,
	};

	enum class STATE
	{
		CHOOSE,	//選択中
		CHECK,	//決定確認
		DECIDE,	//決定
	};

	enum class ENTER_KEY
	{
		PAD_BTN_RIGHT, //Bボタン
		PAT_BTN_DOWN,  //Aボタン
	};

	enum class LAST_CHANGE_NUM
	{
		NONE,
		ALL,	//すべて変更
		ONE,	//1つだけ変更
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

	std::vector<KeyConfig::JOYPAD_BTN> notButton; //反応させないボタン
	ENTER_KEY enterKey_; //決定キー
	KeyConfig::JOYPAD_BTN lastPushBtn_; //最後に押されたボタン
	CONTROL_TYPE controlType_; //操作方法
	CONTROL_TYPE lastChangeType_; //最後に変更した操作方法
	LAST_CHANGE_NUM lastChangeNum_; //最後に変更する数
	STATE state_; //状態

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

	void ChangeControlType(CONTROL_TYPE type);
	std::map<CONTROL_TYPE, std::function<void(void)>> changeControlType_; //状態変更時の関数格納用
	void ChangeControlTypeEnter(void);
	void ChangeControlTypeAvoid(void);
	void ChangeControlTypeJump(void);
	void ChangeControlTypeAttack(void);
	void ChangeControlTypeRockOn(void);
	void ChangeControlTypeMax(void);
	std::function<void(void)> updateControlType_; //操作方法更新時の関数格納用
	void UpdateControlTypeEnter(void);
	void UpdateControlTypeAvoid(void);
	void UpdateControlTypeJump(void);
	void UpdateControlTypeAttack(void);
	void UpdateControlTypeRockOn(void);
	void UpdateControlTypeMax(void);

	void LastChangeControlType(CONTROL_TYPE type);

	void SetControlType(CONTROL_TYPE type, KeyConfig::JOYPAD_BTN lastPushBtn);
	void SetControlType(KeyConfig::CONTROL_TYPE type, KeyConfig::JOYPAD_BTN lastPushBtn);
	std::string GetBtnName(KeyConfig::JOYPAD_BTN btn);
	int GetBtnImage(KeyConfig::JOYPAD_BTN btn);
};