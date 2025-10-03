#pragma once
#include "AttackBase.h"

class CrossLine;

class CrossAttack :    public AttackBase
{
public:
	static constexpr float COOL_DOWN = 30.0f;	//クールダウン
	static constexpr float TIME = 15.0f; //発動時間
	static constexpr float SECOND_TO_DEGREE = 60.0f; //1秒で回転する角度
	static constexpr int LINE_NUM = 4; //クロスラインの数
	static constexpr int LINE_POINT_NUM = 8; //クロスラインの点の数

	CrossAttack(EnemyBase& enemy);
	~CrossAttack(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	int GetCrossLineNum(void) { return static_cast<int>(crossLines_.size()); }
	Transform& GetLineTransform(int lineNum);
private:

	std::vector<std::unique_ptr<CrossLine>> crossLines_; //クロスライン
	float time_;
	float radian_; //現在の回転角度
	void ChangeStateNone(void) override;	//実行されていない
	void ChangeStateReady(void) override;	//実行準備
	void ChangeStateStart(void) override;	//実行開始
	void ChangeStateUpdate(void) override;	//実行中
	void ChangeStateFinish(void) override;	//実行終了

	void UpdateStateNone(void)override;		//実行されていない
	void UpdateStateReady(void)override;	//実行準備
	void UpdateStateStart(void)override;	//実行開始
	void UpdateStateUpdate(void)override;	//実行中
	void UpdateStateFinish(void)override;	//実行終了
};

