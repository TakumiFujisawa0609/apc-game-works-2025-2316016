#pragma once
#include "AttackBase.h"
class FallDownAttack :    public AttackBase
{
public:
	static constexpr float COOL_DOWN = 15.0f;

	FallDownAttack(EnemyBase& enemy);
	~FallDownAttack(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;

private:


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

