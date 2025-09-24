#pragma once
#include "../../Common/Vector2.h"

class EnemyBase;

class EnemyHPUI
{
public:

	static constexpr int MARGIN_X = 200;
	static constexpr int MARGIN_Y = 20;
	static constexpr int SIZE_Y = 30;

	EnemyHPUI(float maxHP ,EnemyBase& enemy);
	~EnemyHPUI();
	void Init(void);
	void Update(void);
	void Draw(void);
private:
	EnemyBase& enemy_;

	float maxHP_;
	Vector2 pos_;
	Vector2 size_;
};

