#pragma once
#include "EnemyBase.h"
class EnemyClear :
    public EnemyBase
{
public:

	EnemyClear(void);
	~EnemyClear(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	void UIDraw(void)override;

	void SetPos(VECTOR pos);
	void SetRot(VECTOR rot);
	void SetScl(VECTOR scl);
protected:
	float disolve_;
};

