#pragma once
#include <DxLib.h>
#include <map>
#include <functional>
#include <vector>
#include "../ObjectModelBase.h"

class Gravity;
class Geometry;
class AttackBase;
class AnimationController;
class Dragon;

class EnemyBase : public ObjectModelBase
{
public:

	//デフォルトの色
	static constexpr COLOR_F DEFAULT_COLOR = { 1.0f,1.0f,1.0f,1.0f };
	static constexpr COLOR_F DAMAGE_COLOR_ADD = { 0.7f,0.1f,0.0f,0.0f };

	//ダメージ関連

	static constexpr float DAMAGE_EFECT_RADIUS = 20.0f;	//ダメージ状態のエフェクト範囲
	static constexpr float DAMAGE_EFECT_TIME = 0.5f;	//ダメージエフェクトの時間

	enum class STATE
	{
		IDLE,	//待機
		ATTACK, //攻撃
		DEAD,   //死亡
	};

	enum class ATTACK_TYPE	//攻撃の種類
	{
		JUMP,		//jamp攻撃(波 同時)
		JUMP_CONSTANT, //jamp攻撃(波 連続)
		FOLLOW,		//追尾攻撃(追従弾)
		FALL_DOWN,	//落下攻撃(落下弾)
		CROSS_LINE,	//十字	
		THUNDER_AROUND, //周囲雷
		WATER_SPRIT,	//水しぶき
		MAX
	};

	enum class ATTACK_STATE
	{
		READY,	//準備
		PLAY,	//実行
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name=""></param>
	EnemyBase(std::weak_ptr<Transform> target);

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

	void OnHit(const std::weak_ptr<Collider> _hitCol, VECTOR hitPos)override;

	Gravity& GetGravity(void) { return *gravity_; }

	void Damage(float damage); //ダメージ処理

	float GetMaxHP(void) const { return maxHP_; }
	float GetHP(void) const { return hp_; }

	void ChangeState(STATE state);

	int GetAttackNum(void) { return static_cast<int>(attackList_.size()); }

	AnimationController& GetAnimController(void);
	void AddAttack(ATTACK_TYPE type);	//攻撃を追加
	void DeleteAttack(ATTACK_TYPE type); //攻撃を削除
	void AllDeleteAttack(void); //全ての攻撃を削除
	int GetAnimNumber(ATTACK_STATE state, ATTACK_TYPE type);
protected:

	std::unique_ptr<Dragon> dragon_; //ドラゴン

	float damageTime_;
	VECTOR hitPos_;

	STATE state_; //状態
	std::unique_ptr<Gravity> gravity_; //重力

	std::weak_ptr<Transform> target_; //ターゲット
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


	virtual void AplayChangeStateFunc(void);

	virtual void InitAddAttack(void);

private:


};

