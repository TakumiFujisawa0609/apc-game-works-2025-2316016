#pragma once
#include <DxLib.h>
#include <map>
#include <functional>
#include <vector>
#include "../ObjectBase.h"

class Gravity;
class AttackBase;
class AnimationController;

class EnemyBase : public ObjectBase
{
public:

	//デフォルトの色
	static constexpr COLOR_F DEFAULT_COLOR = { 1.0f,1.0f,1.0f,1.0f };

	//ダメージ関連
	static constexpr float DAMAGE_EFECT_TIME = 0.6f;	//ダメージのエフェクトの時間

	static constexpr float MODEL_SIZE = 0.5f;

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

	enum class ANIM_TYPE_DRAGON
	{
		IDLE_1,	//待機1
		FLY_FORWARD, //前飛び
		BASIC_ATTACK, //基本攻撃
		TAKE_OFF,	//飛び立つ
		LAND,		//着地
		SCREAM,		//叫び
		CLAW_ATTACK, //爪攻撃
		FLAME_ATTACK, //炎攻撃
		DEFEND,		//防御
		HIT,		//ヒット
		SLEEP,		//睡眠
		WALK,		//歩き
		RUN,		//走り
		FLY_FLAME,	//飛び炎
		FLY_GLIDE,	//飛び滑空
		IDLE_2,	//待機2
		DIE,		//死亡
		FLY_FLOAT,	//飛び浮遊
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

	Transform& GetTransform(void) { return *transform_; }

	Gravity& GetGravity(void) { return *gravity_; }

	void Damage(float damage); //ダメージ処理

	float GetMaxHP(void) const { return maxHP_; }
	float GetHP(void) const { return hp_; }

	void ChangeState(STATE state);

	int GetAttackNum(void) { return static_cast<int>(attackList_.size()); }
	AttackBase& GetAttack(int num) { return *attackList_[num]; }


	void AddAttack(ATTACK_TYPE type);	//攻撃を追加
	void DeleteAttack(ATTACK_TYPE type); //攻撃を削除
	void AllDeleteAttack(void); //全ての攻撃を削除
protected:

	STATE state_; //状態
	std::unique_ptr<Gravity> gravity_; //重力

	Transform& target_; //ターゲット
	std::vector<std::unique_ptr<AttackBase>> attackList_; //攻撃リスト

	std::unique_ptr<AnimationController> animCtrl_;	//アニメーションコントローラー
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

	void InitAnimationControllerDragon(void); //ドラゴン用アニメーションコントローラー初期化
private:


};

