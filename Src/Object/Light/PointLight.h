#pragma once
#include "../ObjectBase.h"
class PointLight :  public ObjectBase
{
public:

	static constexpr float MAX_RADIUS = 100.0f;
	static constexpr float MIN_RADIUS = 0.0f;

	PointLight(Transform& pTrans);
	~PointLight(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	VECTOR& GetLightPos(void) { return pTrans_.pos; }
	float& GetRadius(void) { return radius_; }
	void SetRadiusPercent(float per) { radius_ = (MAX_RADIUS - MIN_RADIUS) * per + MIN_RADIUS; }
private:
	Transform& pTrans_;
	float radius_;
};

