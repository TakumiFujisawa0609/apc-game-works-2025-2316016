#pragma once
#include "../SceneBase.h"
#include "../../Common/Vector2.h"

class PlayerBase;
class SkyDome;
class ShockWave;
class Floor;
class EnemyBase;
class EnemyHPUI;
class PixelMaterial;
class PixelRenderer;

class SceneGame :public SceneBase
{
public:

	//メンバー関数
	//初期化処理(初回の1度のみ実行される)
	SceneGame(void) ;

	virtual ~SceneGame(void) override;

	virtual bool Init(void) override;

	//更新処理
	virtual void Update(void) override;

	//描画処理
	virtual void Draw(void) override;

	void DebugDraw(void);
protected:

	//ヴィネットの最大の強さ
	static constexpr float VIGNETTE_MAX_POW = 5.0f;

	//ヴィネットの点滅用じかん
	float vignetteTime_;

	//プレイヤー
	std::unique_ptr<PlayerBase> player_;
	//敵
	std::unique_ptr<EnemyBase> enemy_;
	//スカイドーム
	std::unique_ptr<SkyDome> skyDome_;
	//敵HPUI
	std::unique_ptr<EnemyHPUI> enemyHPUI_;
	//ステージ
	std::unique_ptr<ShockWave> shockWave_;
	std::unique_ptr<Floor> floor_;
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