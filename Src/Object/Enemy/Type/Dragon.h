#pragma once
#include "EnemyTypeBase.h"

class Dragon : public EnemyTypeBase
{
public:

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



	enum class ANIM_TYPE 
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

	Dragon(EnemyBase& parent);
	virtual ~Dragon(void)override;

	int GetIdleAnim(void)override { return static_cast<int>(ANIM_TYPE::IDLE_1); }
private:
	void InitGeometry(void)override;
	void InitAnimationController(void)override; //アニメーションコントローラー初期化
	void InitAnimMap(void)override; //アニメーション情報マップ初期化
};

