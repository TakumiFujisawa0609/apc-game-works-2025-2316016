#pragma once
#include <memory>
#include <DxLib.h>

class Transform;

class FallDownShot
{
public:
	enum class STATE
	{
		FALL,
		BLAST,
		DEAD,
	};

	static constexpr float SLOW_SPEED = 3.0f;
	static constexpr float FAST_SPEED = 10.0f;

	static constexpr float DAMAGE = 30.0f;
	static constexpr float RADIUS_MIN = 200.0f;
	static constexpr float RADIUS_MAX = 400.0f;
	static constexpr int VERTEX_NUM = 64;
	static constexpr int ALPHA = 128;
	static constexpr float BLAST_TIME = 0.5f;

	FallDownShot(void);
	~FallDownShot(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	STATE GetState(void) const { return state_; }
	Transform& GetTransform(void) { return *transform_; }
	float GetRadius(void) { return radius_; }
	void Hit(void);
private:
	//é©ï™ÇÃ
	std::unique_ptr<Transform> transform_;

	float radius_;	//îºåa
	float initY;	//èâä˙Yç¿ïW
	float blastTime_;
	STATE state_;
	float speed_;
	float InitSpeed(void);
	VECTOR InitPos(void);
};

