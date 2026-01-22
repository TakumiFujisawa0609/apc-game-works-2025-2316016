#pragma once
#include "EnemyBase.h"

class EnemyGame :  public EnemyBase
{
public:
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
protected:

	std::unique_ptr<Gravity> gravity_; //重力

	std::unique_ptr<EnemyAttackManager>attackManager_;

	std::weak_ptr<Transform> target_; //ターゲット

	//体力
	float maxHP_; //最大体力
	float hp_; //体力

	void MoveLimit(void); //移動制限
	void AplayGravity(void);	//重力適用

	virtual void ChangeStateIdle(void)override;	//待機
	virtual void ChangeStateAttack(void)override; //攻撃
	virtual void ChangeStateDead(void)override;   //死亡

	virtual void UpdateIdle(void)override; //待機
	virtual void UpdateAttack(void)override; //攻撃
	virtual void UpdateDead(void)override; //死亡

	virtual void InitAddAttack(void);
	virtual void AplayChangeStateFunc(void)override;
};

