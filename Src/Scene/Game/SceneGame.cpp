#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../../Utility/Utility.h"
#include "../../Application.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SoundManager.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/DrawTranslucentManager.h"
#include "../../Manager/KeyConfig.h"
#include "../../Manager/Camera.h"
#include "../../Manager/DataBank.h"
#include "../../Manager/CollisionManager.h"
#include "../../Renderer/PixelMaterial.h"
#include "../../Renderer/PixelRenderer.h"
#include "../../Object/Common/Gravity.h"
#include "../../Object/Player/GamePlayer.h"
#include "../../Object/SkyDome/SkyDome.h"
#include "../../Object/Stage/Stage.h"
#include "../../Object/Player/PlayerShot.h"
#include "../../Object/Enemy/EnemyBase.h"
#include "../../Object/UI/EnemyHPUI.h"
#include "../../Object/UI/PadUI.h"
#include "../../Object/Log/GameLog.h"
#include "SceneGame.h"

SceneGame::SceneGame(void)
{
	vignetteTime_ = 0.0f;
	time_ = 0.0f;
	postEffectScreen_ = -1;
}

SceneGame::~SceneGame(void)
{
	DeleteGraph(postEffectScreen_);
	SoundManager& ins = SoundManager::GetInstance();
	ins.Stop(SoundManager::SRC::GAME_BGM);
	GameLog::Propaty propaty;
	propaty.time_ = time_;
	propaty.damage_ = player_->GetSumDamage();
	propaty.damageNum_ = player_->GetDamageNum();
	propaty.rollAvoidNum_ = player_->GetAvoidNum();
	propaty.rollAvoidSaccessNum_ = player_->GetAvoidSaccessNum();
	propaty.jumpAvoidNum_ = player_->GetJumpNum();
	propaty.jumpAvoidSaccessNum_ = player_->GetJumpSaccessNum();
	propaty.lastEnemyHP_ = enemy_->GetHP();
	propaty.lastPlayerHP_ = player_->GetHP();
	GameLog::OutPut(propaty);
}


//初期化処理(初回の1度のみ実行される)
bool SceneGame::Init(void)
{
	SceneBase::Init();
	time_ = 0.0f;
	//プレイヤー生成
	player_ = std::make_unique<GamePlayer>(0);
	player_->Init();
	//敵生成
	enemy_ = std::make_unique<EnemyBase>(player_->GetTransform());
	enemy_->Init();
	player_->SetEnemyTransform(enemy_->GetTransform());
	//敵HPUI生成
	enemyHPUI_ = std::make_unique<EnemyHPUI>(enemy_->GetMaxHP(), *enemy_);
	enemyHPUI_->Init();
	//カメラ設定
	auto& cam = SceneManager::GetInstance().GetCamera();
	cam.SetFollow(player_->GetTransform().lock(), enemy_->GetTransform().lock());
	cam.ChangeMode(Camera::MODE::TWO_TARGET_FOLLOW);
	cam.SetPos(player_->GetTransform().lock()->pos);
	//スカイドーム
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->SetColor(SKY_COL);
	skyDome_->Init();

	padUI_ = std::make_unique<PadUI>();
	padUI_->Init();

	//ステージ
	stage_ = std::make_unique<Stage>(player_->GetPointLight());

	// ポストエフェクト用スクリーン
	postEffectScreen_ = MakeScreen(
		Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);
	// ポストエフェクト用(ビネット)
	vineMaterial_ = std::make_unique<PixelMaterial>("Vignette.cso", 1);
	vineMaterial_->AddConstBuf({ VIGNETTE_MAX_POW, player_->GetHP(), PlayerBase::MAX_HP, vignetteTime_});
	vineMaterial_->AddTextureBuf(SceneManager::GetInstance().GetMainScreen());
	vineRenderer_ = std::make_unique<PixelRenderer>(*vineMaterial_);
	vineRenderer_->MakeSquereVertex(
		Vector2(0, 0),
		Vector2(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y)
	);
	vignetteTime_ = 0.0f;

	SoundManager& ins = SoundManager::GetInstance();
	ins.Play(SoundManager::SRC::GAME_BGM,Sound::TIMES::LOOP);
	return true;
}

//更新処理
void SceneGame::Update(void)
{
	//padUI_->Update();
	//スカイドーム
	skyDome_->Update();
	//ステージ
	stage_->Update();
	ChangeCameraMode();
	//プレイヤー
	player_->Update();
	//敵
	enemy_->Update();

	KeyConfig& ins = KeyConfig::GetInstance();
	//全ての更新が終わったら当たり判定をする
	CheckCollision();
	CollisionManager::GetInstance().Update();
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();
	vignetteTime_ += deltaTime;
	time_ += deltaTime;
	enemyHPUI_->Update();
	ChangeScene();

	//メニューを開くキーが押されたらメニューを開く
	if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::OPEN_MENU, KeyConfig::JOYPAD_NO::PAD1))
	{
		SceneManager::GetInstance().PushScene(SceneManager::SCENE_ID::MENU);
	}
	//ヴィネットの定数バッファを更新する
	vineMaterial_->SetConstBuf(0,{ VIGNETTE_MAX_POW, player_->GetHP(), PlayerBase::MAX_HP, vignetteTime_ });

}



//描画処理
void SceneGame::Draw(void)
{
	int mainScreen = SceneManager::GetInstance().GetMainScreen();

	//各オブジェクトの描画
	skyDome_->Draw();
	stage_->Draw();
	player_->Draw();
	enemy_->Draw();

	DrawEffekseer3D();
	//DebugDraw();
	DrawTranslucentManager::GetInstance().Draw();
	//ポストエフェクト用のスクリーンに変える
	SetDrawScreen(postEffectScreen_);

	// 画面を初期化
	ClearDrawScreen();

	vineRenderer_->Draw();

	// メインに戻す
	SetDrawScreen(mainScreen);
	DrawGraph(0, 0, postEffectScreen_, false);

	//シェーダーの効果を受けないUI等を描画
	enemyHPUI_->Draw();
	//DrawFormatString(0, 0, 0, "%f", player_->GetHP());
	//DrawFormatString(0, 20, 0, "%d", GetDrawCallCount());
	//player_->UIDraw();
	enemy_->UIDraw();
	//padUI_->Draw();
}

void SceneGame::Load(void)
{
	SoundManager& ins = SoundManager::GetInstance();
	ins.Load(SoundManager::SRC::FIRE);
	ins.Load(SoundManager::SRC::PSHOT_HIT);
	ins.Load(SoundManager::SRC::PSHOT_THROW);
	ins.Load(SoundManager::SRC::GAME_BGM);
}

void SceneGame::DebugDraw(void)
{
}

void SceneGame::ChangeCameraMode(void)
{
	auto& cam = SceneManager::GetInstance().GetCamera();
	auto& ins = KeyConfig::GetInstance();
	auto lockOnType = DataBank::GetInstance().GetLockOnType();
	auto controlType = DataBank::GetInstance().GetControlType();
	switch (lockOnType)
	{
	case DataBank::LOCK_ON_TYPE::FIXED:
		break;
	case DataBank::LOCK_ON_TYPE::PRESS:
		if (ins.IsNew(KeyConfig::CONTROL_TYPE::CHENGE_CAMERA_MODE, KeyConfig::JOYPAD_NO::PAD1, controlType))
		{
			cam.ChangeMode(Camera::MODE::TWO_TARGET_FOLLOW);
		}
		else if (ins.IsTrgUp(KeyConfig::CONTROL_TYPE::CHENGE_CAMERA_MODE, KeyConfig::JOYPAD_NO::PAD1, controlType))
		{
			cam.ChangeMode(Camera::MODE::FOLLOW);
		}
		break;
	case DataBank::LOCK_ON_TYPE::SWITCH:
		if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::CHENGE_CAMERA_MODE, KeyConfig::JOYPAD_NO::PAD1, controlType))
		{
			cam.ChangeMode(cam.GetMode() == Camera::MODE::FOLLOW ? Camera::MODE::TWO_TARGET_FOLLOW : Camera::MODE::FOLLOW);
		}
		break;
	default:
		break;
	}

}

void SceneGame::CheckCollision(void)
{
	VECTOR pPos = player_->GetTransform().lock()->pos;
	pPos.y = 0.0f;
	auto dis = Utility::Distance(pPos, Utility::VECTOR_ZERO);
	if (dis > Stage::RADIUS)
	{
		VECTOR dir = VNorm(VSub(pPos,Utility::VECTOR_ZERO));
		VECTOR pos = VScale(dir, Stage::RADIUS);
		pos.y = player_->GetTransform().lock()->pos.y;
		player_->SetPos(pos);
	}
}

void SceneGame::ChangeScene(void)
{
	//敵のHPが0を切ったらゲームクリアに移動
	if (enemy_->GetHP() <= 0.0f)
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMECLEAR, true);
	}
	//プレイヤーが死んだらゲームオーバーに移動
	if (player_->GetState() == PlayerBase::STATE::DEAD)
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER, true);
	}

}