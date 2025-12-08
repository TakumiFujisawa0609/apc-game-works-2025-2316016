#pragma once
#include "EnemyBase.h"
class EnemyTitle :  public EnemyBase
{
public:

	EnemyTitle(std::weak_ptr<Transform> trans);
	~EnemyTitle(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	void UIDraw(void)override;

	void SetPos(VECTOR pos);
	void SetRot(VECTOR rot);
	void SetScl(VECTOR scl);
protected:
	void InitAddAttack(void)override;
};

