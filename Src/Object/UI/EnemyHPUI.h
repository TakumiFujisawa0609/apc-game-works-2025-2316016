#pragma once
#include "../../Common/Vector2.h"
#include "../../Application.h"

class EnemyBase;
class PixelMaterial;
class PixelRenderer;

class EnemyHPUI
{
public:

	static constexpr int MARGIN_X = 100;	//Xの余白
	static constexpr int MARGIN_Y = 10;		//Yの余白
	static constexpr int SIZE_Y = 80;		//Yの大きさ
	static constexpr VECTOR GAGE_COL = { 0.1f,1.0f,0.1f };

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
	void InitMaskImageMinMax(void);
private:
	// ポストエフェクト用(ビネット)
	std::unique_ptr<PixelMaterial> uiMaterial_;
	std::unique_ptr<PixelRenderer> uiRenderer_;

	//敵のクラス
	EnemyBase& enemy_;

	//最大体力
	float maxHP_;
	//左上座標
	Vector2 pos_;
	//大きさ
	Vector2 size_;
	//体力パーセント
	float rate_;

	//画面ハンドル
	int screenHandle_;
	//UI画像
	int uiHandle_;
	//マスク画像
	int uiMaskHandle_;

	float gageMin_;
	float gageMax_;
};

