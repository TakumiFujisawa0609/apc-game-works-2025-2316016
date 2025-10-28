#pragma once
#include <DxLib.h>
#include <map>
#include <functional>
#include "../../EnemyBase.h"
#include "../../../ObjectBase.h"

class Transform;

class AttackBase : public ObjectBase
{
public:

	//攻撃優先範囲
	static constexpr float SHORT_RANGE = 500.0f;  //近距離300以内なら優先して発動される
	static constexpr float MIDDLE_RANGE = 800.0f; //中距離600以内なら優先して発動される
	static constexpr float LONG_RANGE = 1100.0f;   //遠距離1000以内なら優先して発動される

	enum class RANGE	//優先度
	{
		SHORT, //近距離
		MIDDLE,//中距離
		LONG,  //遠距離
		ALL,   //全距離
	};

	enum class STATE
	{
		NONE,	//実行されていない
		READY, //実行準備
		START,	//実行開始
		UPDATE, //実行中
		FINISH, //実行終了
	};

	enum class GEOMETORY
	{
		SPHERE,	//球
		CIRCLE,	//円
		CIRCUMFERENCE,	//円周
		MODEL,	//モデル
	};

	AttackBase(EnemyBase& enemy);
	virtual ~AttackBase(void)override;
	virtual void Init(void)override;
	virtual void Update(void)override;
	virtual void Draw(void)override;
	virtual int GetSubObjectNum(void) const = 0;

	void SetTarget(Transform* target) { target_ = target; }

	RANGE GetRange(void) const { return range_; }

	STATE GetState(void) const { return state_; }

	void ChangeState(STATE state);

	GEOMETORY GetGeometory(void) const { return geo_; }

	EnemyBase::ATTACK_TYPE GetMyType(void)const { return myType_; }
protected:
	EnemyBase& enemy_; //親のEnemyBase

	EnemyBase::ATTACK_TYPE myType_;
	RANGE range_; //攻撃範囲
	STATE state_; //状態
	Transform* target_; //相手のTransform
	float deleyTime_; //攻撃のディレイ時間
	GEOMETORY geo_;

	std::map<STATE, std::function<void(void)>> changeState_; //状態変更時の関数格納用
	virtual void ChangeStateNone(void);		//実行されていない
	virtual void ChangeStateReady(void);	//実行準備
	virtual void ChangeStateStart(void);	//実行開始
	virtual void ChangeStateUpdate(void);	//実行中
	virtual void ChangeStateFinish(void);	//実行終了

	std::function<void(void)> updateState_;
	virtual void UpdateStateNone(void);		//実行されていない
	virtual void UpdateStateReady(void);	//実行準備
	virtual void UpdateStateStart(void);	//実行開始
	virtual void UpdateStateUpdate(void);	//実行中
	virtual void UpdateStateFinish(void);	//実行終了


private:
};

