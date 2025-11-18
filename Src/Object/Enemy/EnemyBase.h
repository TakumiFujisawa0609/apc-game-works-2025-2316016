#pragma once
#include <DxLib.h>
#include <map>
#include <functional>
#include <vector>
#include "../ObjectBase.h"

class Gravity;
class Geometry;
class AttackBase;
class AnimationController;

class EnemyBase : public ObjectBase
{
public:

	//デフォルトの色
	static constexpr COLOR_F DEFAULT_COLOR = { 1.0f,1.0f,1.0f,1.0f };
	static constexpr COLOR_F DAMAGE_COLOR_ADD = { 0.7f,0.1f,0.0f,0.0f };

	//ダメージ関連
	static constexpr float MODEL_SIZE = 0.5f;

	//当たり判定関連
	static constexpr float SMALL_RADIUS = 25.0f;	//手の先。足の先、頭等の小さい当たり判定
	static constexpr int HAND_L_BORN_NUM = 29;	//左手のボーン番号
	static constexpr int ARM_L_BORN_NUM = 28;	//左腕のボーン番号
	static constexpr int HAND_R_BORN_NUM = 43;	//右手のボーン番号
	static constexpr int ARM_R_BORN_NUM = 42;	//右腕のボーン番号
	static constexpr int LEG_L_BORN_NUM = 95;	//左足のボーン番号
	static constexpr int FOOT_L_BORN_NUM = 94;	//左脚のボーン番号
	static constexpr int LEG_R_BORN_NUM = 109;	//右足のボーン番号
	static constexpr int FOOT_R_BORN_NUM = 108;	//右脚のボーン番号
	static constexpr int HEAD_BORN_NUM = 14;	//頭のボーン番号

	static constexpr int WING_L_BASE_BORN_NUM = 57;	//左羽の基盤ボーン番号
	static constexpr int WING_L_1_BORN_NUM = 60;	//左羽の1つ目のボーン番号
	static constexpr int WING_L_2_BORN_NUM = 64;	//左羽の2つ目のボーン番号
	static constexpr int WING_L_3_BORN_NUM = 72;	//左羽の3つ目のボーン番号
	static constexpr int WING_L_4_BORN_NUM = 68;	//左羽の4つ目のボーン番号
	static constexpr int WING_L_5_BORN_NUM = 73;	//左羽の5つ目のボーン番号
	static constexpr int WING_L_NEAR_BODY_BORN_NUM = 55;

	static constexpr int WING_R_BASE_BORN_NUM = 76;	//右羽の基盤ボーン番号
	static constexpr int WING_R_1_BORN_NUM = 79;	//右羽の1つ目のボーン番号
	static constexpr int WING_R_2_BORN_NUM = 83;	//右羽の2つ目のボーン番号
	static constexpr int WING_R_3_BORN_NUM = 91;	//右羽の3つ目のボーン番号
	static constexpr int WING_R_4_BORN_NUM = 87;	//右羽の4つ目のボーン番号
	static constexpr int WING_R_5_BORN_NUM = 92;	//右羽の5つ目のボーン番号
	static constexpr int WING_R_NEAR_BODY_BORN_NUM = 74;

	static constexpr float BIG_RADIUS = 50.0f;	//胴体等の大きい当たり判定
	static constexpr int BODY_1_BORN_NUM = 2;	//胴体1のボーン番号(尻尾より)
	static constexpr int BODY_2_BORN_NUM = 10;	//胴体2のボーン番号(首より)

	static constexpr float MIDIUM_RADIUS = 30.0f;	//首等の中くらいの当たり判定
	static constexpr int NECK_BORN_NUM = 12;	//首のボーン番号

	static constexpr int TAIL_1_BORN_NUM = 4;	//尻尾１のボーン番号
	static constexpr int TAIL_2_BORN_NUM = 7;	//尻尾２のボーン番号


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

	virtual void UIDraw(void)override;

	void OnHit(const std::weak_ptr<Collider> _hitCol, VECTOR hitPos)override;

	Transform& GetTransform(void) { return *transform_; }

	Gravity& GetGravity(void) { return *gravity_; }

	void Damage(float damage); //ダメージ処理

	float GetMaxHP(void) const { return maxHP_; }
	float GetHP(void) const { return hp_; }

	void ChangeState(STATE state);

	int GetAttackNum(void) { return static_cast<int>(attackList_.size()); }
	AttackBase& GetAttack(int num) { return *attackList_[num]; }
	AnimationController& GetAnimController(void) { return *animCtrl_; }

	void AddAttack(ATTACK_TYPE type);	//攻撃を追加
	void DeleteAttack(ATTACK_TYPE type); //攻撃を削除
	void AllDeleteAttack(void); //全ての攻撃を削除
	void SetAnim(ANIM_TYPE_DRAGON type);	//アニメーションを適用する
protected:

	float damageTime_;
	VECTOR hitPos_;

	STATE state_; //状態
	std::unique_ptr<Gravity> gravity_; //重力

	std::map<int, VECTOR> framePos_;	//フレーム番号とフレーム座標
	Transform& target_; //ターゲット
	std::vector<std::unique_ptr<AttackBase>> attackList_; //攻撃リスト

	std::unique_ptr<AnimationController> animCtrl_;	//アニメーションコントローラー
	//体力
	float maxHP_; //最大体力
	float hp_; //体力

	void MoveLimit(void); //移動制限
	void AplayGravity(void);	//重力適用


	//飛行時の座標
	//VECTOR flyLocalPos_;
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

	virtual void InitGeometry(void);
	virtual void InitAddAttack(void);

	void UpdateFramePos(void);	//framePos_を更新する
private:


};

