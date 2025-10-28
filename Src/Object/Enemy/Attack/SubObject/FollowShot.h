#pragma once
#include <memory>
#include <DxLib.h>
#include "SubObjectBase.h"

class FollowShot : public SubObjectBase
{
public:
	enum class SPEED_TYPE	//速さ種類
	{
		SLOW,	//遅い
		MIDIUM,	//普通
		FAST,	//早い
		RANDOM,	//ランダム
		MAX,
	};

	enum class STATE	//状態
	{
		SHOT,	//追従中
		BLAST,	//爆発
		DEAD,	//破壊
	};

	static constexpr float SLOW_SPEED = 3.0f;	//遅いスピード
	static constexpr float MIDIUM_SPEED = 5.5f;	//普通のスピード
	static constexpr float FAST_SPEED = 10.0f;	//速いスピード

	static constexpr float DAMAGE = 15.0f;	//ダメージ
	static constexpr float ATTACK_TIME = 10.0f;	//攻撃時間

	FollowShot(Transform& target, SPEED_TYPE speed,VECTOR startPos);
	~FollowShot(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	bool IsShot(void) { return state_ == STATE::SHOT; }
	void Hit(void);
private:
	//相手の
	Transform& target_;

	STATE state_;
	float speed_;
	float time_;

	float InitSpeed(SPEED_TYPE speedType) const;

};