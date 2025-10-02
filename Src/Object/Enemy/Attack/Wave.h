#pragma once
#include <DxLib.h>
class Wave
{
public:
	enum class SPEED_TYPE
	{
		SLOW,
		MIDIUM,
		FAST,
		MAX,
	};

	static constexpr float SLOW_SPEED = 200.0f;
	static constexpr float MIDIUM_SPEED = 300.0f;
	static constexpr float FAST_SPEED = 400.0f;
	static constexpr int VERTEX_NUM = 180;

	static constexpr float MAX_RADIUS = 1500.0f;	//îgñ‰ÇÃç≈ëÂîºåa
	static constexpr float DAMAGE = 10.0f;

	Wave(VECTOR centerPos,SPEED_TYPE speedType , int color );
	~Wave(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Reset(void);

	float GetRadius(void) const { return speed_ * time_; }
	VECTOR GetPos(void) const { return centerPos_; }
	bool IsEnd(void) const { return MAX_RADIUS <= speed_ * time_; }
private:
	int color_;
	float speed_;
	VECTOR centerPos_;
	float time_;

	float InitSpeed(SPEED_TYPE speedType) const;

};

