#include<DxLib.h>
#include"../Application.h"
#include"../Manager/SceneManager.h"
#include"../Manager/ResourceManager.h"
#include"../Manager/SoundManager.h"
#include"../Manager/InputManager.h"
#include"../Manager/KeyConfig.h"
#include"../Manager/Camera.h"
#include "../Object/Player/PlayerBase.h"
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
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		players_[i] = std::make_unique<PlayerBase>(i);
		players_[i]->Init();
	}
	auto& cam = SceneManager::GetInstance().GetCamera();
	cam.ChangeMode(Camera::MODE::TWO_TARGET);
	cam.SetFollow(&players_[0]->GetTransform(), &players_[1]->GetTransform());

	return true;
}

//更新処理
void SceneGame::Update(void)
{
	//InputManager& ins = InputManager::GetInstance();
	KeyConfig& ins = KeyConfig::GetInstance();
	for (int i = 0;i < MAX_PLAYER;i++)
	{
		players_[i]->Update();
	}
}

//描画処理
void SceneGame::Draw(void)
{
	for (int i = 0;i < MAX_PLAYER;i++)
	{
		players_[i]->Draw();
	}
	DrawLine3D(players_[0]->GetTransform().pos, players_[1]->GetTransform().pos, GetColor(255, 0, 255));
}

void SceneGame::DebugDraw(void)
{
}


