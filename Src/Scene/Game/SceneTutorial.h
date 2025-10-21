#pragma once
#include <functional>
#include <map>
#include "SceneGame.h"
class SceneTutorial :  public SceneGame
{
public:

	enum class STEP
	{
		MOVE,		//移動
		LOCK_ON,	//ロックオン
		ATTACK,		//攻撃
		DAMAGE,		//ダメージ
		AVOID,		//回避
		JUMP,		//ジャンプ
		MENU,		//メニュー
		FINISH,
	};

	enum class STATE
	{
		EXPLANATION,	//説明
		ACTION,			//行動
		CLEAR,			//クリア
	};

	SceneTutorial(void);
	virtual ~SceneTutorial(void) override;
	virtual bool Init(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;

private:
	STEP step_;	//チュートリアルの進行度
	void ChangeStep(STEP step);
	std::map<STEP, std::function<void(void)>> changeStep_;	//ステップ変更関数格納
	void ChangeStepMove(void);
	void ChangeStepLockOn(void);
	void ChangeStepAttack(void);
	void ChangeStepDamage(void);
	void ChangeStepAvoid(void);
	void ChangeStepJump(void);
	void ChangeStepMenu(void);
	void ChangeStepFinish(void);
	std::function<void(void)>  updateStep_;	//更新関数格納
	void UpdateStepMove(void);
	void UpdateStepLockOn(void);
	void UpdateStepAttack(void);
	void UpdateStepDamage(void);
	void UpdateStepAvoid(void);
	void UpdateStepJump(void);
	void UpdateStepMenu(void);
	void UpdateStepFinish(void);
	std::function<void(void)> drawStep_;;	//描画関数格納
	void DrawStepMove(void);
	void DrawStepLockOn(void);
	void DrawStepAttack(void);
	void DrawStepDamage(void);
	void DrawStepAvoid(void);
	void DrawStepJump(void);
	void DrawStepMenu(void);
	void DrawStepFinish(void);
	STATE state_;			//チュートリアルの状態

	void ChangeScene(void) override;
};

