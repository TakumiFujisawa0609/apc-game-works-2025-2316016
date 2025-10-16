#pragma once
#include <DxLib.h>
#include <memory>

class Gravity;
class Transform;

class PlayerShot
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

	~PlayerShot();
	void Init(void);
	void Update(void);
	void Draw(void);
	bool IsDead(void) const { return isDead_; }
	Transform& GetTransform(void) { return *transform_; }
	float GetRadius(void) { return RADIUS; }
	void Hit(void);
	bool IsShot() const { return state_ == STATE::SHOT; }
private:
	//情報
	std::unique_ptr<Transform> transform_;
	//重力
	std::unique_ptr<Gravity> gravity_;
	//状態
	STATE state_;
	//ターゲット座標
	VECTOR targetPos_;
	//始点
	VECTOR startPos_;
	//死んだか
	bool isDead_;

};

