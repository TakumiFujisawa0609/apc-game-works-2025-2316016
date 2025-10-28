#pragma once
#include "AttackBase.h"
#include "../SubObject/FallDownShot.h"

class FallDownAttack :    public AttackBase
{
public:
	static constexpr float COOL_DOWN = 60.0f;	//クールダウン
	static constexpr int MAX_FALL_NUM = 30; //落下弾の同時に出せる最大数
	static constexpr float TIME = 30.0f; //落下弾が落ち続ける時間

	FallDownAttack(EnemyBase& enemy);
	~FallDownAttack(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	int GetFallDownShotNum(void) { return static_cast<int>(fallDownShots_.size()); }
	Transform& GetShotTransform(int shotNum);
	FallDownShot::STATE GetShotState(int shotNum);
	float GetShotRadius(int shotNum);

private:

	std::vector<std::unique_ptr<FallDownShot>> fallDownShots_; //落下弾

	float time_;		//落下弾が落ち続ける時間

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

