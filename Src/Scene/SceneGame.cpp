#include<DxLib.h>
#include"../Application.h"
#include"../Manager/SceneManager.h"
#include"../Manager/ResourceManager.h"
#include"../Manager/SoundManager.h"
#include"../Manager/InputManager.h"
#include"../Manager/KeyConfig.h"
#include"../Manager/Camera.h"
#include "../Object/Player/PlayerBase.h"
#include "../Object/SkyDome/SkyDome.h"
#include "../Object/Player/PlayerShot.h"
#include "../Object/Enemy/EnemyBase.h"
#include "../Object/UI/EnemyHPUI.h"
#include"SceneGame.h"
#include "../Utility/Utility.h"

SceneGame::SceneGame(void)
{
}

SceneGame::~SceneGame(void)
{

}


//初期化処理(初回の1度のみ実行される)
bool SceneGame::Init(void)
{
	SceneBase::Init();
	//プレイヤー生成
	player_ = std::make_unique<PlayerBase>(0);
	player_->Init();
	//敵生成
	enemy_ = std::make_unique<EnemyBase>(player_->GetTransform());
	enemy_->Init();
	//敵HPUI生成
	enemyHPUI_ = std::make_unique<EnemyHPUI>(enemy_->GetMaxHP(), *enemy_);
	enemyHPUI_->Init();
	//カメラ設定
	auto& cam = SceneManager::GetInstance().GetCamera();
	cam.ChangeMode(Camera::MODE::FOLLOW);
	cam.SetFollow(&player_->GetTransform(), &enemy_->GetTransform());
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Init();
	return true;
}

//更新処理
void SceneGame::Update(void)
{
	skyDome_->Update();
	//InputManager& ins = InputManager::GetInstance();
	KeyConfig& ins = KeyConfig::GetInstance();
	ChangeCameraMode();
	//プレイヤー
	player_->Update();
	//敵
	enemy_->Update();
};

//描画処理
void SceneGame::Draw(void)
{


	skyDome_->Draw();
	player_->Draw();
	enemy_->Draw();
	DrawLine3D(player_->GetTransform().pos, enemy_->GetTransform().pos, GetColor(255, 0, 255));
	DebugDraw();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawCube3D(PlayerBase::MOVE_LIMIT_MIN, PlayerBase::MOVE_LIMIT_MAX, GetColor(255, 255, 255), GetColor(0, 0, 255), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	enemyHPUI_->Draw();
}

void SceneGame::DebugDraw(void)
{
	VECTOR min = PlayerBase::MOVE_LIMIT_MIN;
	VECTOR max = PlayerBase::MOVE_LIMIT_MAX;
	std::vector<VECTOR> vertexes;
	vertexes.push_back(VGet(min.x, min.y, min.z));
	vertexes.push_back(VGet(min.x, min.y, max.z));
	vertexes.push_back(VGet(max.x, min.y, min.z));
	vertexes.push_back(VGet(max.x, min.y, max.z));
	vertexes.push_back(VGet(min.x, max.y, min.z));
	vertexes.push_back(VGet(min.x, max.y, max.z));
	vertexes.push_back(VGet(max.x, max.y, min.z));
	vertexes.push_back(VGet(max.x, max.y, max.z));
	for (auto& vertex : vertexes)
	{
		DrawSphere3D(vertex, 16.0f, 32, 0xff00ff, 0xff00ff, true);
	}
	for (int i = 0; i < 4; i++)
	{
		DrawLine3D(vertexes[2 * i], vertexes[2 * i + 1], 0x0000ff);
		DrawLine3D(vertexes[i], vertexes[i + 4], 0x0000ff);
	}
	DrawLine3D(vertexes[0], vertexes[2], 0x0000ff);
	DrawLine3D(vertexes[1], vertexes[3], 0x0000ff);
	DrawLine3D(vertexes[4], vertexes[6], 0x0000ff);
	DrawLine3D(vertexes[5], vertexes[7], 0x0000ff);
}

void SceneGame::ChangeCameraMode(void)
{
	auto& cam = SceneManager::GetInstance().GetCamera();
	auto& ins = KeyConfig::GetInstance();
	if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::CHENGE_CAMERA_MODE, KeyConfig::JOYPAD_NO::PAD1))
	{
		cam.ChangeMode(Camera::MODE::TWO_TARGET_FOLLOW);
	}
	else if (ins.IsTrgUp(KeyConfig::CONTROL_TYPE::CHENGE_CAMERA_MODE, KeyConfig::JOYPAD_NO::PAD1))
	{
		cam.ChangeMode(Camera::MODE::FOLLOW);
	}
}

void SceneGame::CheckCollision(void)
{
	
}


