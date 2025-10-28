#pragma once
#include <vector>
#include <memory>
#include "AttackBase.h"
#include "../SubObject/FollowShot.h"

class FollowAttack :    public AttackBase
{
public:
	static constexpr float COOL_DOWN = 3.0f;	//クールダウン
	static constexpr float RADIUS = 20.0f;		//半径
	static constexpr int RANDOM_SHOT_NUM = 5;	//ランダムスピードの数

	FollowAttack(EnemyBase& enemy);
	~FollowAttack(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	int GetShotNum(void) { return static_cast<int>(shots_.size()); }
	Transform& GetShotTransform(int shotNum);
	float GetRadius(void) { return RADIUS; }
	void HitShot(int shotNum) { shots_[shotNum]->Hit(); }
private:
	std::vector<std::unique_ptr<FollowShot>> shots_;
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

