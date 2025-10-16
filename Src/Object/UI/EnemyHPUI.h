#pragma once
#include "../../Common/Vector2.h"

class EnemyBase;

class EnemyHPUI
{
public:

	static constexpr int MARGIN_X = 200;	//Xの余白
	static constexpr int MARGIN_Y = 20;		//Yの余白
	static constexpr int SIZE_Y = 30;		//Yの大きさ

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="maxHP">最大体力</param>
	/// <param name="enemy">敵の参照型</param>
	EnemyHPUI(float maxHP ,EnemyBase& enemy);
	~EnemyHPUI();
	void Init(void);
	void Update(void);
	void Draw(void);
private:
	//敵のクラス
	EnemyBase& enemy_;

	//最大体力
	float maxHP_;
	//左上座標
	Vector2 pos_;
	//大きさ
	Vector2 size_;
};

