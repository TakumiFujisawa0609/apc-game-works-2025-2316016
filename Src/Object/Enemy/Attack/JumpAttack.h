#pragma once
#include <memory>
#include <vector>
#include "AttackBase.h"

class Wave;

class JumpAttack :public AttackBase
{
public:
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

};

