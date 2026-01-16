#pragma once
#include "../SceneBase.h"
#include "../../Common/Vector2.h"

class GamePlayer;
class SkyDome;
class Stage;
class PadUI;
class EnemyBase;
class EnemyHPUI;
class PixelMaterial;
class PixelRenderer;

class SceneGame :public SceneBase
{
public:

	static constexpr FLOAT4 SKY_COL = { 1.0f,0.1f,1.0f,1.0f };
	static constexpr int PAD_IMAGE_SIZE_X = 465;
	static constexpr int PAD_IMAGE_SIZE_Y = 256;

	//メンバー関数
	//初期化処理(初回の1度のみ実行される)
	SceneGame(void) ;

	virtual ~SceneGame(void) override;

	virtual bool Init(void) override;

	//更新処理
	virtual void Update(void) override;

	//描画処理
	virtual void Draw(void) override;

	virtual void Load(void)override;

	void DebugDraw(void);
protected:

	//ヴィネットの最大の強さ
	static constexpr float VIGNETTE_MAX_POW = 5.0f;

	//ヴィネットの点滅用じかん
	float vignetteTime_;

	//総時間
	float time_;

	//プレイヤー
	std::unique_ptr<GamePlayer> player_;
	//敵
	std::unique_ptr<EnemyBase> enemy_;
	//スカイドーム
	std::unique_ptr<SkyDome> skyDome_;
	//PadUI
	std::unique_ptr<PadUI> padUI_;
	//敵HPUI
	std::unique_ptr<EnemyHPUI> enemyHPUI_;
	//ステージ
	std::unique_ptr<Stage> stage_;
	// ポストエフェクト用スクリーン
	int postEffectScreen_;

	// ポストエフェクト用(ビネット)
	std::unique_ptr<PixelMaterial> vineMaterial_;
	std::unique_ptr<PixelRenderer> vineRenderer_;


	//カメラの変更
	void ChangeCameraMode(void);
	//当たり判定
	void CheckCollision(void);
	//シーン変更
	virtual void ChangeScene(void);
};