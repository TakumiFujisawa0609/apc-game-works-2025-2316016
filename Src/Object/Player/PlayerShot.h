#pragma once
#include <DxLib.h>
#include <memory>

class Gravity;
class Transform;

class PlayerShot
{
public:
	static constexpr float SPEED = 20.0f; //弾の速度
	static constexpr float POWER = 10.0f; //玉の初期パワー
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
private:
	std::unique_ptr<Transform> transform_;
	std::unique_ptr<Gravity> gravity_;
	VECTOR targetPos_;
	VECTOR startPos_;
	bool isDead_;

};

