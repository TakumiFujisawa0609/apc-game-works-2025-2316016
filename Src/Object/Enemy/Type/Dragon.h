#pragma once
#include <map>
#include "../../ObjectBase.h"
#include "../EnemyAttackManager.h"

class AnimationController;
class EnemyBase;

class Dragon : public ObjectBase
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

	struct ANIM_INFO
	{
		EnemyAttackManager::ATTACK_STATE attackState; //攻撃状態
		EnemyAttackManager::ATTACK_TYPE attackType; //攻撃の種類
		//bool operator>(const ANIM_INFO& info);
		//const bool operator<(const ANIM_INFO& info);
		//const bool operator<=(const ANIM_INFO& info);
		//const bool operator>=(const ANIM_INFO& info);
		//const bool operator==(const ANIM_INFO& info);
		// 比較子は const な参照を受け取り、const メソッドにする
		bool operator>(const ANIM_INFO& info) const noexcept {
			return info < *this;
		}

		bool operator<(const ANIM_INFO& info) const noexcept {
			if (attackState != info.attackState) return attackState < info.attackState;
			return attackType < info.attackType;
		}

		bool operator<=(const ANIM_INFO& info) const noexcept {
			return !(*this > info);
		}

		bool operator>=(const ANIM_INFO& info) const noexcept {
			return !(*this < info);
		}

		bool operator==(const ANIM_INFO& info) const noexcept {
			return attackState == info.attackState && attackType == info.attackType;
		}

		bool operator!=(const ANIM_INFO& info) const noexcept {
			return !(*this == info);
		}
	};

	Dragon(EnemyBase& parent);
	~Dragon(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	void UIDraw(void)override;
	void OnHit(const std::weak_ptr<Collider> _hitCol, VECTOR hitPos)override;

	void SetAnim(ANIM_TYPE type);	//アニメーションを適用する
	AnimationController& GetAnimController(void) { return *animCtrl_; }
	int GetAnimType(EnemyAttackManager::ATTACK_STATE attackState, EnemyAttackManager::ATTACK_TYPE attackType);
	int GetIdleAnim(void) { return static_cast<int>(ANIM_TYPE::IDLE_1); }
private:
	EnemyBase& parent_; //親EnemyBase
	std::unique_ptr<AnimationController> animCtrl_;	//アニメーションコントローラー
	std::map<int, VECTOR> framePos_;	//フレーム番号とフレーム座標
	std::map<ANIM_INFO, ANIM_TYPE> animInfoMap_; //アニメーション情報マップ
	void InitGeometry(void);
	void InitAnimationController(void); //アニメーションコントローラー初期化
	void InitAnimMap(void); //アニメーション情報マップ初期化
	void AddAnimInfoMap(EnemyAttackManager::ATTACK_STATE attackState, EnemyAttackManager::ATTACK_TYPE attackType,ANIM_TYPE animType );
	void UpdateFramePos(void);	//framePos_を更新する
};

