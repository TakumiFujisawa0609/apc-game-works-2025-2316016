#pragma once
#include "EnemyTypeBase.h"

class Chicken : public EnemyTypeBase
{
public:

	static constexpr float MODEL_SIZE = 10.5f;

	//当たり判定関連
	static constexpr int CENTER_BORN_NUM = 9;	//中心のボーン番号(胴体)
	static constexpr int BOTTOM_BORN_NUM = 10;	//底部のボーン番号(足元)

	static constexpr int ARM_L_S_BORN_NUM = 19;	//左腕のボーン番号(肩)
	static constexpr int ARM_L_E_BORN_NUM = 21;	//左腕のボーン番号(先端)

	static constexpr int ARM_R_S_BORN_NUM = 25;	//右腕のボーン番号(肩)
	static constexpr int ARM_R_E_BORN_NUM = 27;	//右腕のボーン番号(先端)

	static constexpr float LEG_RADIUS = 50.0f;	//足の当たり判定半径

	static constexpr int LEG_L_S_BORN_NUM = 16;	//左足のボーン番号(付け根)
	static constexpr int LEG_L_C_BORN_NUM = 17;	//左足のボーン番号(かかと)
	static constexpr int LEG_L_E_BORN_NUM = 18;	//左足のボーン番号(先端)

	static constexpr int LEG_R_S_BORN_NUM = 22;	//右足のボーン番号(付け根)
	static constexpr int LEG_R_C_BORN_NUM = 23;	//右足のボーン番号(かかと)
	static constexpr int LEG_R_E_BORN_NUM = 24;	//右足のボーン番号(先端)


	enum class ANIM_TYPE
	{
		IDLE,	//待機
		WALK, //歩き
		ATTACK, //攻撃
		FALL,	//落下
		RANK1,		//ランク1
		RANK2,		//ランク2
		RANK3,		//ランク3
		RANK4,		//ランク4
		DAMAGE,		//ダメージ
		RUN,		//走り
		DEATH,		//死亡
		PUNCH,		//パンチ
		JUMP,		//ジャンプ
		LAND,		//着地
		RIDE_1,		//乗る1
		HOLD,		//ホールド
		MAX
	};

	Chicken(EnemyBase& parent);
	~Chicken(void)override;

	int GetIdleAnim(void)override { return static_cast<int>(ANIM_TYPE::IDLE); }
private:
	void InitGeometry(void)override;
	void InitAnimationController(void)override; //アニメーションコントローラー初期化
	void InitAnimMap(void)override; //アニメーション情報マップ初期化
};

