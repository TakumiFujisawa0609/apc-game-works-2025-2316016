#pragma once
#include "AttackBase.h"

class WaterSprit;

class WaterSpritAttack : public AttackBase
{
public:
	static constexpr int MAX_WATER_SPRIT_WAVE_NUM = 10;	//最大ウォータースプリット波数
	static constexpr int ONE_WATER_SPRIT_NUM = 8;		//1波のウォータースプリット数
	static constexpr float MAX_SPEED = 20.0f;			//最大速度
	static constexpr float MIN_SPEED = 3.0f;	//最遅速度
	static constexpr float INTERVAL_TIME = 0.5f; //波の間隔時間
	static constexpr float COOL_DOWN = 30.0f; //クールダウン時間
	WaterSpritAttack(EnemyBase& enemy);
	~WaterSpritAttack() override;
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	int GetSubObjectNum(void)const override { return static_cast<int>(waterSprit_.size()); }
private:

	std::vector<std::unique_ptr<WaterSprit>> waterSprit_;
	int waveNum_;
	float intervalTime_;
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

	void CreateWaterSpritWave(void);
};

