#pragma once
#include "../../Common/Vector2.h"
#include "../../Application.h"
#include "../ObjectPixelBase.h"

class EnemyBase;

class EnemyHPUI : public ObjectPixelBase
{
public:

	static constexpr int MARGIN_X = 100;	//Xの余白
	static constexpr int MARGIN_Y = 10;		//Yの余白
	static constexpr int SIZE_Y = 80;		//Yの大きさ
	static constexpr VECTOR GAGE_COL = { 0.1f,1.0f,0.1f };
	static constexpr VECTOR INIT_LIGHT_DIR = { 0.0f,-0.3f,1.0f };
	static constexpr float ROT_DEG_SEC = 90.0f;	//1秒に回る量
	static constexpr float LIGHT_TIME = 2.0f;
	static constexpr float SHAKE_TIME_MAX = 0.5f;
	static constexpr float SHAKE_POWER_PER_SEC = 300.0f;
	static constexpr float ANGLE_MAX = 10.0f;
	static constexpr float ANGLE_MIN = -10.0f;

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

	//敵のクラス
	EnemyBase& enemy_;

	VECTOR lightDir_;
	//最大体力
	float maxHP_;
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
	//法線画像
	int uiNormalHandle_;

	float gageMin_;
	float gageMax_;

	float angle_;
	int shakeSign_;
	float shakeTime_;
};

