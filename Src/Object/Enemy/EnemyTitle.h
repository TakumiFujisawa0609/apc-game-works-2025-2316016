#pragma once
#include "EnemyBase.h"
class EnemyTitle :  public EnemyBase
{
public:
	static constexpr int ANIM_POP = 180;

	EnemyTitle(void);
	~EnemyTitle(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	void UIDraw(void)override;

	void SetPos(VECTOR pos);
	void SetRot(VECTOR rot);
	void SetScl(VECTOR scl);
protected:
};

