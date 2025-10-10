#pragma once
#include <vector>
#include <map>
#include <functional>
#include "../../Manager/KeyConfig.h"

class Control
{
public:
	enum class CONTROL_TYPE
	{
		ENTER,	//決定
		AVOID,	//回避
		JUMP,	//ジャンプ
		ATTACK,	//攻撃
		//LOCK_ON_TYPE, //ロックオンの種類
		LOCK_ON,	//ロックオン
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



	Control();
	~Control();

	void Init(void);
	void Update(void);
	void Draw(void);

	bool IsEnd(void) const { return isEnd_; }
private:

	KeyConfig& ins_ = KeyConfig::GetInstance();
	bool isEnd_ ; //終了したかどうか

	std::vector<KeyConfig::JOYPAD_BTN> notButton; //反応させないボタン
	ENTER_KEY enterKey_; //決定キー
	KeyConfig::JOYPAD_BTN lastPushBtn_; //最後に押されたボタン
	CONTROL_TYPE controlType_; //操作方法
	CONTROL_TYPE lastChangeType_; //最後に変更した操作方法
	LAST_CHANGE_NUM lastChangeNum_; //最後に変更する数
	STATE state_; //状態

	void ChangeControlType(CONTROL_TYPE type);
	std::map<CONTROL_TYPE, std::function<void(void)>> changeControlType_; //状態変更時の関数格納用
	void ChangeControlTypeEnter(void);
	void ChangeControlTypeAvoid(void);
	void ChangeControlTypeJump(void);
	void ChangeControlTypeAttack(void);
	void ChangeControlTypeLockOnType(void);
	void ChangeControlTypeLockOn(void);
	void ChangeControlTypeMax(void);
	std::function<void(void)> updateControlType_; //操作方法更新時の関数格納用
	void UpdateControlTypeEnter(void);
	void UpdateControlTypeAvoid(void);
	void UpdateControlTypeJump(void);
	void UpdateControlTypeAttack(void);
	void UpdateControlTypeLockOnType(void);
	void UpdateControlTypeLockOn(void);
	void UpdateControlTypeMax(void);

	void LastChangeControlType(CONTROL_TYPE type);
	void SetControlType(CONTROL_TYPE type, KeyConfig::JOYPAD_BTN lastPushBtn);

	void SetControlType(KeyConfig::CONTROL_TYPE type, KeyConfig::JOYPAD_BTN lastPushBtn);

	void ChooseUpdate(void);
	void CheckUpdate(void);
};

