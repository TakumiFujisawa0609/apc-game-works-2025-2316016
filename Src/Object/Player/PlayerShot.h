#pragma once
#include <DxLib.h>
#include "../ObjectBase.h"

class Gravity;

class PlayerShot : public ObjectBase
{
public:

	enum class STATE
	{
		SHOT,
		BLAST,
		DEAD,
	};

	static constexpr float SPEED = 20.0f; //弾の速度
	static constexpr float POWER = 15.0f; //玉の初期パワー
	static constexpr float RADIUS = 5.0f;	//球の大きさ
	/// <summary>
	/// クリエイト
	/// </summary>
	/// <param name="pPos">プレイヤーの座標</param>
	/// <param name="tPos">ターゲット座標</param>
	PlayerShot(VECTOR pPos,VECTOR tPos);

	~PlayerShot()override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	bool IsDead(void) const { return isDead_; }
	float GetRadius(void)const { return RADIUS; }
	void Hit(void);
	bool IsShot(void) const { return state_ == STATE::SHOT; }
private:
	//重力
	std::unique_ptr<Gravity> gravity_;
	//状態
	STATE state_;
	//ターゲット座標
	VECTOR targetPos_;
	//始点
	VECTOR startPos_;
	//方向
	VECTOR dir_;
	//死んだか
	bool isDead_;

};

