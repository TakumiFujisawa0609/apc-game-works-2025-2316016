#pragma once
#include <memory>
#include <vector>
#include "AttackBase.h"
#include "../SubObject/Wave.h"

class JumpAttack :public AttackBase
{
public:
	static constexpr float COOL_DOWN = 8.0f;	//クールダウン
	static constexpr float JUMP_POW = 30.0f; //ジャンプの高さ
	static constexpr int RANDOM_WAVE_NUM = 5; //ランダム波紋の出せる最大数
	static constexpr float TIME_SCALE = 3.0f;
	static constexpr FLOAT4 COLOR = { 0.5f,1.0f,0.5f,0.5f };

	JumpAttack(EnemyAttackManager& parent);
	~JumpAttack(void) override;
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	int GetSubObjectNum(void)override { return static_cast<int>(wave_.size()); }
private:
	float time_;
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

