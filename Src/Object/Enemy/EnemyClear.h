#pragma once
#include "EnemyBase.h"
class EnemyClear :
    public EnemyBase
{
public:
	static constexpr float DISOLVE_MAX = 1.0f;
	static constexpr float DISOLVE_TIME = 5.0f;

	EnemyClear(std::weak_ptr<Transform> trans);
	~EnemyClear(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	void UIDraw(void)override;

	void SetPos(VECTOR pos);
	void SetRot(VECTOR rot);
	void SetScl(VECTOR scl);
protected:
	void InitAddAttack(void)override;
	float disolve_;
};

