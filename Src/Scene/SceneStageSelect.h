#pragma once
#include <DxLib.h>
#include <vector>
#include <map>
#include <functional>
#include <memory>
#include "../../Manager/KeyConfig.h"
#include "SceneBase.h"

class PlayerBase;
class SkyDome;
class Stage;
class Gate;

class SceneStageSelect : public SceneBase
{
public:

	static constexpr int GATE_NUM = 4;
	static constexpr float GATE_RADIUS = 1000.0f;

	//メンバー関数
	SceneStageSelect(void);

	~SceneStageSelect(void) override;

	//初期化処理(初回の1度のみ実行される)
	bool Init(void)override;

	//更新処理
	void Update(void) override;

	//描画処理
	void Draw(void) override;

private:

	std::unique_ptr<SkyDome> skyDome_;
	std::unique_ptr<PlayerBase> player_;
	std::unique_ptr<Stage> stage_;
	std::unique_ptr<Gate> gate_[GATE_NUM];

};