#include<DxLib.h>
#include<cmath>
#include"../Application.h"
#include"../Manager/Scenemanager.h"
#include"../Manager/SoundManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/KeyConfig.h"
#include "../Manager/Camera.h"
#include "../Manager/ResourceManager.h"
#include"SceneTitle.h"


SceneTitle::SceneTitle(void)
{
}

SceneTitle::~SceneTitle(void)
{

}

//初期化処理(初回の1度のみ実行される)
bool SceneTitle::Init(void)
{
	return true;
}

//更新処理
void SceneTitle::Update(void)
{
	//ゲームシーンへ
	if (KeyConfig::GetInstance().IsNew(KeyConfig::CONTROL_TYPE::ENTER,KeyConfig::JOYPAD_NO::PAD1))
	{
		SoundManager::GetInstance().Play(SoundManager::SRC::ENTER_SOUND, Sound::TIMES::ONCE);
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME, true);
	}
	
}
//描画処理
void SceneTitle::Draw(void)
{
	DrawString(100, 100, "Title", GetColor(255, 255, 255));
}
