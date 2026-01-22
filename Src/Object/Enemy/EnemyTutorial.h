#pragma once
#include "EnemyGame.h"
class EnemyTutorial : public EnemyGame
{
public:
	EnemyTutorial(std::weak_ptr<Transform> target);
	~EnemyTutorial() override;
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
private:
	void InitAddAttack(void)override;
};

