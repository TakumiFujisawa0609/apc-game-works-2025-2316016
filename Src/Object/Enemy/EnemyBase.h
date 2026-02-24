#pragma once
#include <DxLib.h>
#include <map>
#include <functional>
#include <vector>
#include "../ObjectModelBase.h"

class Gravity;
class Geometry;
class AnimationController;
class EnemyTypeBase;
class EnemyAttackManager;

class EnemyBase : public ObjectModelBase
{
public:

	//デフォルトの色
	static constexpr COLOR_F DEFAULT_COLOR = { 1.0f,1.0f,1.0f,1.0f };
	static constexpr COLOR_F DAMAGE_COLOR_ADD = { 0.7f,0.1f,0.0f,0.0f };

	//ダメージ関連

	static constexpr float DAMAGE_EFECT_RADIUS = 20.0f;	//ダメージ状態のエフェクト範囲
	static constexpr float DAMAGE_EFECT_TIME = 0.5f;	//ダメージエフェクトの時間

	//溶解エフェクト関連
	static constexpr float DISOLVE_MAX = 1.0f;
	static constexpr float DISOLVE_TIME = 5.0f;
	static constexpr float INIT_DISOLVE = -0.01f;
	static constexpr float NOISE_SCALE = 5.0f;
	enum class STATE
	{
		IDLE,	//待機
		ATTACK, //攻撃
		BREAK,	//壊れ
		DEAD,   //死亡
	};


	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name=""></param>
	EnemyBase(void);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	virtual ~EnemyBase(void)override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name=""></param>
	virtual void Init(void)override;

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name=""></param>
	virtual void Update(void)override;

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name=""></param>
	virtual void Draw(void)override;

	virtual void UIDraw(void)override;

	virtual void OnHit(const std::weak_ptr<Collider> _hitCol, VECTOR hitPos)override;

	void ChangeState(STATE state);

	AnimationController& GetAnimController(void);

	EnemyTypeBase& GetModelType(void) { return *enemyType_; }
protected:

	STATE state_; //状態
	std::unique_ptr<EnemyTypeBase> enemyType_; //敵の種類
	float damageTime_;
	VECTOR hitPos_;

	//状態変更用
	std::map<STATE, std::function<void(void)>> changeState_; //状態変更時の関数格納用
	virtual void ChangeStateIdle(void);	//待機
	virtual void ChangeStateAttack(void); //攻撃
	virtual void ChangeStateBreak(void);	//壊れ
	virtual void ChangeStateDead(void);   //死亡

	std::function<void(void)> updateState_; //更新処理用関数
	virtual void UpdateIdle(void); //待機
	virtual void UpdateAttack(void); //攻撃
	virtual void UpdateBreak(void);	//壊れ
	virtual void UpdateDead(void); //死亡

	virtual void AplayChangeStateFunc(void);
private:


};

