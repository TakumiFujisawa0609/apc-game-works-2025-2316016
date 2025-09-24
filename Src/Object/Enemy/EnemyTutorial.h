#pragma once
#include "EnemyBase.h"
class EnemyTutorial :
    public EnemyBase
{
public:
	EnemyTutorial(Transform& target);
	~EnemyTutorial() override;
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
};

