#pragma once
#include <DxLib.h>
#include <memory>
#include <map>
#include <functional>
#include <vector>

class Transform;
class Gravity;
class AttackBase;

class EnemyBase
{
public:

	//ダメージ関連
	static constexpr float DAMAGE_EFECT_TIME = 0.6f;	//ダメージのエフェクトの時間

	static constexpr float MODEL_SIZE = 5.0f;

	enum class STATE
	{
		IDLE,	//待機
		ATTACK, //攻撃
		DEAD,   //死亡
	};

	enum class ATTACK_TYPE
	{
		JUMP,
		MAX
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name=""></param>
	EnemyBase(Transform& target);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	virtual ~EnemyBase(void);

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name=""></param>
	virtual void Init(void);

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name=""></param>
	virtual void Update(void);

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name=""></param>
	virtual void Draw(void);

	Transform& GetTransform(void) { return *transform_; }

	Gravity& GetGravity(void) { return *gravity_; }

	void Damage(float damage); //ダメージ処理

	float GetMaxHP(void) const { return maxHP_; }
	float GetHP(void) const { return hp_; }

	void ChangeState(STATE state);

protected:

	//基本情報
	std::unique_ptr<Transform> transform_;
	STATE state_; //状態
	std::unique_ptr<Gravity> gravity_; //重力

	Transform& target_; //ターゲット
	std::vector<std::unique_ptr<AttackBase>> attackList_; //攻撃リスト
	//体力
	float maxHP_; //最大体力
	float hp_; //体力

	void MoveLimit(void); //移動制限
	void AplayGravity(void);	//重力適用

	//状態変更用
	std::map<STATE, std::function<void(void)>> changeState_; //状態変更時の関数格納用
	virtual void ChangeStateIdle(void);	//待機
	virtual void ChangeStateAttack(void); //攻撃
	virtual void ChangeStateDead(void);   //死亡

	std::function<void(void)> updateState_; //更新処理用関数
	virtual void UpdateIdle(void); //待機
	virtual void UpdateAttack(void); //攻撃
	virtual void UpdateDead(void); //死亡

	void AddAttack(ATTACK_TYPE type);	//攻撃を追加

	virtual void AplayChangeStateFunc(void);

private:


};

