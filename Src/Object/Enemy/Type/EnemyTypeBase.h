#pragma once
#include <map>
#include "../../ObjectBase.h"
#include "../EnemyAttackManager.h"

class AnimationController;
class EnemyBase;

class EnemyTypeBase : public ObjectBase
{
public:

	enum class TYPE
	{
		CHICKEN,	//チキン
		DRAGON,		//ドラゴン
		MAX
	};

	struct ANIM_INFO
	{
		EnemyAttackManager::ATTACK_STATE attackState; //攻撃状態
		EnemyAttackManager::ATTACK_TYPE attackType; //攻撃の種類
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

	EnemyTypeBase(EnemyBase& parent);
	~EnemyTypeBase(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	void UIDraw(void)override;
	void OnHit(const std::weak_ptr<Collider> _hitCol, VECTOR hitPos)override;
	TYPE GetType(void)const { return type_; }
	void SetAnim(int type);	//アニメーションを適用する
	AnimationController& GetAnimController(void) { return *animCtrl_; }
	int GetAnimType(EnemyAttackManager::ATTACK_STATE attackState, EnemyAttackManager::ATTACK_TYPE attackType);
	virtual int GetIdleAnim(void) = 0;
	virtual int GetBrakAnim(void) = 0;
protected:
	TYPE type_; //敵の種類
	EnemyBase& parent_; //親EnemyBase
	std::unique_ptr<AnimationController> animCtrl_;	//アニメーションコントローラー
	std::map<int, VECTOR> framePos_;	//フレーム番号とフレーム座標
	std::map<ANIM_INFO, int> animInfoMap_; //アニメーション情報マップ
	virtual void InitGeometry(void) = 0;
	virtual void InitAnimationController(void) = 0; //アニメーションコントローラー初期化
	virtual void InitAnimMap(void) = 0; //アニメーション情報マップ初期化
	void AddAnimInfoMap(EnemyAttackManager::ATTACK_STATE attackState, EnemyAttackManager::ATTACK_TYPE attackType,int animType);
	void UpdateFramePos(void);	//framePos_を更新する
};

