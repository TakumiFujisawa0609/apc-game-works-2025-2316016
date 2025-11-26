//#pragma once
#include "SceneBase.h"
#include <memory>
#include <vector>
#include "../Common/Vector2.h"

class SkyDome;
class TitlePlayer;
class EnemyTitle;

class SceneTitle: public SceneBase
{
public:

	static constexpr VECTOR CAMERA_POS = { 0.0f,000.0f,-1000.0f };
	static constexpr VECTOR CAMERA_TARGET_POS = { 0.0f,0.0f,0.0f };
	static constexpr VECTOR ENEMY_SIZE = { 0.2f,0.2f,0.2f };
	static constexpr float MARGIN = 50.0f;
	//メンバー関数
	SceneTitle(void);

	~SceneTitle(void) override;

	//初期化処理(初回の1度のみ実行される)
	bool Init(void)override;

	//更新処理
	void Update(void) override;

	//描画処理
	void Draw(void) override;

	void Load(void)override;
private:
	std::unique_ptr<TitlePlayer> player_;
	std::unique_ptr<SkyDome> skyDome_;
	std::unique_ptr<EnemyTitle> enemy_;
};