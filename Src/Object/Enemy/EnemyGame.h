#pragma once
#include "EnemyBase.h"

class EnemyGame :  public EnemyBase
{
public:

	static constexpr int WING_LOW_DAMAGE_NUM = 5; //羽のダメージ数(この数だけダメージを受けると羽が壊れる)
	static constexpr int WING_HIGH_DAMAGE_NUM = 10; //羽のダメージ数(この数だけダメージを受けると羽が完全に壊れる)
	static constexpr float WING_DAMAGE = 4.0f; //羽に与えるダメージ
	static constexpr float WING_LOW_DAMAGE_RATE = 0.5f; //羽が壊れたときのダメージ減少率
	static constexpr float WING_HIGH_DAMAGE_RATE = 0.25f; //羽が完全に壊れたときのダメージ減少率
	static constexpr float BODY_DAMAGE = 2.0f; //体に与えるダメージ
	static constexpr float WING_HIGH_DAMAGE_BODY_DAMAGE_RATE = 3.0f; //羽が完全に壊れたときに体に与えるダメージ

	//ダメージ状態
	enum class WING_DAMAGE_STATE
	{
		NONE,
		LOW,
		HIGH,
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name=""></param>
	EnemyGame(std::weak_ptr<Transform> target);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	virtual ~EnemyGame(void)override;

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

	void OnHit(const std::weak_ptr<Collider> _hitCol, VECTOR hitPos)override;

	Gravity& GetGravity(void) { return *gravity_; }

	void Damage(float damage); //ダメージ処理

	float GetMaxHP(void) const { return maxHP_; }
	float GetHP(void) const { return hp_; }

	bool IsEnd(void) const { return isEnd_; }
protected:

	std::unique_ptr<Gravity> gravity_; //重力

	std::unique_ptr<EnemyAttackManager>attackManager_;

	std::weak_ptr<Transform> target_; //ターゲット

	//体力
	float maxHP_; //最大体力
	float hp_; //体力

	WING_DAMAGE_STATE wingDamageState_; //羽のダメージ状態
	int wingDamageNum_; //羽のダメージ数

	float wingHitDamage_; //羽に当たったときのダメージ
	float bodyHitDamage_; //体に当たったときのダメージ

	float disolve_;
	bool isEnd_; //終了判定

	void MoveLimit(void); //移動制限
	void AplayGravity(void);	//重力適用
	void UpdateWingDamageState(void); //羽のダメージ状態更新

	virtual void ChangeStateIdle(void)override;	//待機
	virtual void ChangeStateAttack(void)override; //攻撃
	virtual void ChangeStateDead(void)override;   //死亡

	virtual void UpdateIdle(void)override; //待機
	virtual void UpdateAttack(void)override; //攻撃
	virtual void UpdateDead(void)override; //死亡

	virtual void InitAddAttack(void);
	virtual void AplayChangeStateFunc(void)override;
};

