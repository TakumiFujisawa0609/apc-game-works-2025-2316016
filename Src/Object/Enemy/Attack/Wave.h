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

	static constexpr float SLOW_SPEED = 500.0f;
	static constexpr float MIDIUM_SPEED = 700.0f;
	static constexpr float FAST_SPEED = 1000.0f;
	static constexpr int VERTEX_NUM = 180;

	static constexpr float MAX_RADIUS = 2500.0f;	//îgñ‰ÇÃç≈ëÂîºåa

	Wave(VECTOR centerPos,SPEED_TYPE speedType , int color );
	~Wave();
	void Init(void);
	void Update(void);
	void Draw(void);
	void Reset(void);

	float GetRadius(void) const { return speed_ * time_; }

	bool IsEnd(void) const { return MAX_RADIUS <= speed_ * time_; }
private:
	int color_;
	float speed_;
	VECTOR centerPos_;
	float time_;

	float InitSpeed(SPEED_TYPE speedType) const;

};

