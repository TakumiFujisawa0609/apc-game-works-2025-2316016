#include <DxLib.h>
#include "../../Utility/Utility.h"
#include "../../Application.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SoundManager.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/KeyConfig.h"
#include "../../Manager/Camera.h"
#include "../../Manager/DataBank.h"
#include "../../Renderer/PixelMaterial.h"
#include "../../Renderer/PixelRenderer.h"
#include "../../Object/Player/PlayerBase.h"
#include "../../Object/SkyDome/SkyDome.h"
#include "../../Object/Stage/ShockWave.h"
#include "../../Object/Stage/Floor.h"
#include "../../Object/Player/PlayerShot.h"
#include "../../Object/Enemy/EnemyBase.h"
#include "../../Object/Enemy/EnemyTutorial.h"
#include "../../Object/Enemy/Attack/AttackBase.h"
#include "../../Object/Enemy/Attack/JumpAttack.h"
#include "../../Object/Enemy/Attack/JumpAttackConstant.h"
#include "../../Object/Enemy/Attack/FollowAttack.h"
#include "../../Object/Enemy/Attack/FallDownAttack.h"
#include "../../Object/Enemy/Attack/CrossAttack.h"
#include "../../Object/Enemy/Attack/ThunderAroundAttack.h"
#include "../../Object/Enemy/Attack/Wave.h"
#include "../../Object/Enemy/Attack/FollowShot.h"
#include "../../Object/Enemy/Attack/FallDownShot.h"
#include "../../Object/Enemy/Attack/CrossLine.h"
#include "../../Object/Enemy/Attack/ThunderAround.h"
#include "../../Object/UI/EnemyHPUI.h"
#include "SceneTutorial.h"

SceneTutorial::SceneTutorial(void)
{
	changeStep_[STEP::MOVE] = std::bind(&SceneTutorial::ChangeStepMove, this);
	changeStep_[STEP::LOCK_ON] = std::bind(&SceneTutorial::ChangeStepLockOn, this);
	changeStep_[STEP::ATTACK] = std::bind(&SceneTutorial::ChangeStepAttack, this);
	changeStep_[STEP::DAMAGE] = std::bind(&SceneTutorial::ChangeStepDamage, this);
	changeStep_[STEP::AVOID] = std::bind(&SceneTutorial::ChangeStepAvoid, this);
	changeStep_[STEP::JUMP] = std::bind(&SceneTutorial::ChangeStepJump, this);
	changeStep_[STEP::MENU] = std::bind(&SceneTutorial::ChangeStepMenu, this);
	changeStep_[STEP::FINISH] = std::bind(&SceneTutorial::ChangeStepFinish, this);
}

SceneTutorial::~SceneTutorial(void)
{
}

bool SceneTutorial::Init(void)
{
	SceneBase::Init();
	//プレイヤー生成
	player_ = std::make_unique<PlayerBase>(0);
	player_->Init();
	//敵生成
	enemy_ = std::make_unique<EnemyTutorial>(player_->GetTransform());
	enemy_->Init();
	//敵HPUI生成
	enemyHPUI_ = std::make_unique<EnemyHPUI>(enemy_->GetMaxHP(), *enemy_);
	enemyHPUI_->Init();
	//カメラ設定
	auto& cam = SceneManager::GetInstance().GetCamera();
	cam.SetFollow(&player_->GetTransform(), &enemy_->GetTransform());
	cam.ChangeMode(Camera::MODE::TWO_TARGET_FOLLOW);
	cam.SetPos(player_->GetTransform().pos);
	//スカイドーム
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Init();

	//ステージ
	floor_ = std::make_unique<Floor>();
	floor_->Init();
	shockWave_ = std::make_unique<ShockWave>();
	shockWave_->Init();

	// ポストエフェクト用スクリーン
	postEffectScreen_ = MakeScreen(
		Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);
	// ポストエフェクト用(ビネット)
	vineMaterial_ = std::make_unique<PixelMaterial>("Vignette.cso", 1);
	vineMaterial_->AddConstBuf({ VIGNETTE_MAX_POW, player_->GetHP(), PlayerBase::MAX_HP, vignetteTime_ });
	vineMaterial_->AddTextureBuf(SceneManager::GetInstance().GetMainScreen());
	vineRenderer_ = std::make_unique<PixelRenderer>(*vineMaterial_);
	vineRenderer_->MakeSquereVertex(
		Vector2(0, 0),
		Vector2(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y)
	);
	vignetteTime_ = 0.0f;
	ChangeStep(STEP::MOVE);
	return true;
}

void SceneTutorial::Update(void)
{
	SceneGame::Update();
	updateStep_();
}

void SceneTutorial::Draw(void)
{
	SceneGame::Draw();
	drawStep_();
}

void SceneTutorial::ChangeStep(STEP step)
{
	step_ = step;
	changeStep_[step_]();
}

void SceneTutorial::ChangeStepMove(void)
{
	updateStep_ = std::bind(&SceneTutorial::UpdateStepMove, this);
	drawStep_ = std::bind(&SceneTutorial::DrawStepMove, this);
}

void SceneTutorial::ChangeStepLockOn(void)
{
	updateStep_ = std::bind(&SceneTutorial::UpdateStepLockOn, this);
	drawStep_ = std::bind(&SceneTutorial::DrawStepLockOn, this);
}

void SceneTutorial::ChangeStepAttack(void)
{
	updateStep_ = std::bind(&SceneTutorial::UpdateStepAttack, this);
	drawStep_ = std::bind(&SceneTutorial::DrawStepAttack, this);
}

void SceneTutorial::ChangeStepDamage(void)
{
	updateStep_ = std::bind(&SceneTutorial::UpdateStepDamage, this);
	drawStep_ = std::bind(&SceneTutorial::DrawStepDamage, this);
}

void SceneTutorial::ChangeStepAvoid(void)
{
	updateStep_ = std::bind(&SceneTutorial::UpdateStepAvoid, this);
	drawStep_ = std::bind(&SceneTutorial::DrawStepAvoid, this);
}

void SceneTutorial::ChangeStepJump(void)
{
	updateStep_ = std::bind(&SceneTutorial::UpdateStepJump, this);
	drawStep_ = std::bind(&SceneTutorial::DrawStepJump, this);
}

void SceneTutorial::ChangeStepMenu(void)
{
	updateStep_ = std::bind(&SceneTutorial::UpdateStepMenu, this);
	drawStep_ = std::bind(&SceneTutorial::DrawStepMenu, this);
}

void SceneTutorial::ChangeStepFinish(void)
{
	updateStep_ = std::bind(&SceneTutorial::UpdateStepFinish, this);
	drawStep_ = std::bind(&SceneTutorial::DrawStepFinish, this);
}

void SceneTutorial::UpdateStepMove(void)
{

}

void SceneTutorial::UpdateStepLockOn(void)
{
}

void SceneTutorial::UpdateStepAttack(void)
{
}

void SceneTutorial::UpdateStepDamage(void)
{
}

void SceneTutorial::UpdateStepAvoid(void)
{
}

void SceneTutorial::UpdateStepJump(void)
{
}

void SceneTutorial::UpdateStepMenu(void)
{
}

void SceneTutorial::UpdateStepFinish(void)
{
}

void SceneTutorial::DrawStepMove(void)
{
}

void SceneTutorial::DrawStepLockOn(void)
{
}

void SceneTutorial::DrawStepAttack(void)
{
}

void SceneTutorial::DrawStepDamage(void)
{
}

void SceneTutorial::DrawStepAvoid(void)
{
}

void SceneTutorial::DrawStepJump(void)
{
}

void SceneTutorial::DrawStepMenu(void)
{
}

void SceneTutorial::DrawStepFinish(void)
{
}

void SceneTutorial::ChangeScene(void)
{
}
