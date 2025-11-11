#pragma once
#include <memory>
#include <vector>
#include "AttackBase.h"
#include "../SubObject/Wave.h"
class Wave;

class JumpAttackConstant :    public AttackBase
{
public:
	static constexpr float COOL_DOWN = 15.0f;	//クールダウン
	static constexpr float JUMP_POW = 30.0f; //ジャンプの高さ
	static constexpr float INTERVAL_TIME = 0.4f; //波紋を出す間隔時間
	static constexpr float TIME = 10.0f; //ジャンプしてから波紋を出し終わるまでの時間

	JumpAttackConstant(EnemyBase& enemy);
	~JumpAttackConstant(void) override;
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	int GetSubObjectNum(void)const override { return static_cast<int>(wave_.size()); }
	void GetWaveState(float& radius, VECTOR& centerPos, int waveNum);
	const float GetDamage(void)const override { return Wave::DAMAGE; }

private:
	float time_;
	float intervalTime_;
	bool isDown_;	//下りに入ったか
	std::vector<std::unique_ptr<Wave>> wave_;
	void ChangeStateNone(void) override;		//実行されていない
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

