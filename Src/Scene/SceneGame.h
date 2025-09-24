#pragma once
#include "SceneBase.h"
#include "../Common/Vector2.h"

class PlayerBase;
class SkyDome;
class EnemyBase;
class EnemyHPUI;

class SceneGame :public SceneBase
{
public:

	//メンバー関数
	//-----------------------------------------------------------
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

	//プレイヤー
	std::unique_ptr<PlayerBase> player_;
	//敵
	std::unique_ptr<EnemyBase> enemy_;
	//スカイドーム
	std::unique_ptr<SkyDome> skyDome_;
	//敵HPUI
	std::unique_ptr<EnemyHPUI> enemyHPUI_;

	//カメラの変更
	void ChangeCameraMode(void);
	//当たり判定
	void CheckCollision(void);
};