#include<DxLib.h>
#include"../Application.h"
#include"../Manager/SceneManager.h"
#include"../Manager/ResourceManager.h"
#include"../Manager/SoundManager.h"
#include"../Manager/InputManager.h"
#include"../Manager/KeyConfig.h"
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
	
	return true;
}

//更新処理
void SceneGame::Update(void)
{
	//InputManager& ins = InputManager::GetInstance();
	KeyConfig& ins = KeyConfig::GetInstance();

}

//描画処理
void SceneGame::Draw(void)
{

}

void SceneGame::DebugDraw(void)
{
}


