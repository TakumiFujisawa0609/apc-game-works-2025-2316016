#pragma once
#include <vector>

#include "AttackBase.h"
#include "../SubObject/ThunderAround.h"

class ThunderAroundAttack :    public AttackBase
{
public:
	static constexpr float COOL_DOWN = 60.0f;
	static constexpr float RADIUS = 50.0f;
	static constexpr int THUNDER_NUM = 8;
	static constexpr float DISTANCE = 600.0f; //サンダーの出現距離
	static constexpr float TIME = 20.0f; //攻撃が続く時間
	static constexpr float INTERVAL_TIME = 1.0f; //サンダーが出現する間隔時間
	static constexpr float TEXTURE_SCALE_X = 1.0f;
	static constexpr float TEXTURE_SCALE_Y = 5.0f;
	static constexpr float TIME_SCALE = 3.0f;
	static constexpr float THRESHOLD = 1.5f;

	ThunderAroundAttack(EnemyAttackManager& parent);
	~ThunderAroundAttack(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	int GetSubObjectNum(void)override { return static_cast<int>(thunders_.size()); }
private:

	std::vector<std::unique_ptr<ThunderAround>> thunders_;
	float sumTime_;
	float time_;
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

	void CreateThunder(void);
};

