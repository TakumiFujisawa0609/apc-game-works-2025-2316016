#pragma once
#include <memory>
#include <vector>
#include "AttackBase.h"

class Wave;

class JumpAttack :public AttackBase
{
public:
	static constexpr float COOL_DOWN = 8.0f;	//クールダウン
	static constexpr float JUMP_POW = 30.0f; //ジャンプの高さ

	JumpAttack(EnemyBase& enemy);
	~JumpAttack() override;
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
private:
	void Jump(void);
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

