#pragma once
#include <memory>
#include "SceneBase.h"

class SkyDome;
class ClearPlayer;
class EnemyClear;

class SceneGameOver :   public SceneBase
{
public:
	static constexpr FLOAT4 SKY_COL = { 2.0f,1.0f,0.0f,1.0f };
	static constexpr VECTOR CAMERA_POS = { 0.0f,000.0f,-1000.0f };
	static constexpr VECTOR CAMERA_TARGET_POS = { 0.0f,0.0f,0.0f };
	static constexpr VECTOR ENEMY_SIZE = { 0.15f,0.15f,0.15f };
	static constexpr float MARGIN = 150.0f;

	SceneGameOver(void);

	virtual ~SceneGameOver(void) override;

	virtual bool Init(void) override;

	//çXêVèàóù
	virtual void Update(void) override;

	//ï`âÊèàóù
	virtual void Draw(void) override;
private:
	std::unique_ptr<SkyDome> skyDome_;
	std::unique_ptr<ClearPlayer> player_;
	std::unique_ptr<EnemyClear> enemy_;
};

