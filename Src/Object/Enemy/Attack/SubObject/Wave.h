#pragma once
#include <DxLib.h>
#include "SubObjectBase.h"

class Wave : public SubObjectBase
{
public:
	enum class SPEED_TYPE
	{
		SLOW,
		MIDIUM,
		FAST,
		RANDOM,
		MAX,
	};

	static constexpr float SLOW_SPEED = 150.0f;
	static constexpr float MIDIUM_SPEED = 300.0f;
	static constexpr float FAST_SPEED = 400.0f;
	static constexpr int VERTEX_NUM = 64;

	static constexpr float MAX_RADIUS = 1500.0f;	//îgñ‰ÇÃç≈ëÂîºåa
	static constexpr float DAMAGE = 10.0f;

	static constexpr int HEIGHT_VERTEX_NUM = 2;	//çÇÇ≥í∏ì_êî
	static constexpr float HEIGHT = 30.0f;	//çÇÇ≥

	Wave(VECTOR centerPos,SPEED_TYPE speedType , AttackBase& parent );
	~Wave(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	void Reset(void);

	float GetRadius(void) const { return speed_ * time_; }
	VECTOR GetPos(void) const { return centerPos_; }
	bool IsEnd(void) const { return MAX_RADIUS <= speed_ * time_; }
private:
	float speed_;
	VECTOR centerPos_;
	float time_;

	float InitSpeed(SPEED_TYPE speedType) const;
	void SetPolygonInfo(void) override;
};

