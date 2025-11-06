#include <DxLib.h>
#include <cmath>
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/SoundManager.h"
#include "../Manager/KeyConfig.h"
#include "../Manager/Camera.h"
#include "../Manager/ResourceManager.h"
#include "SceneTitle.h"


SceneTitle::SceneTitle(void)
{
}

SceneTitle::~SceneTitle(void)
{

}

//初期化処理(初回の1度のみ実行される)
bool SceneTitle::Init(void)
{
	auto camera = SceneManager::GetInstance().GetCamera();
	camera.ChangeMode(Camera::MODE::FIXED_POINT);
	return true;
}

//更新処理
void SceneTitle::Update(void)
{
	//ゲームシーンへ
	if (KeyConfig::GetInstance().IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER,KeyConfig::JOYPAD_NO::PAD1))
	{
		SoundManager::GetInstance().Play(SoundManager::SRC::ENTER_SOUND, Sound::TIMES::ONCE);
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME, true);
	}
	
}
//描画処理
void SceneTitle::Draw(void)
{
	DrawString(100, 100, "Title", GetColor(255, 255, 255));
	SetDrawBright(255, 0, 0);
	//DrawExtendGraph(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, ResourceManager::GetInstance().Load(ResourceManager::SRC::TITLE_IMAGE_2).handleId_, true);
	DrawRotaGraph(Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y, 1.0f, 0.0f, ResourceManager::GetInstance().Load(ResourceManager::SRC::TITLE_IMAGE_2).handleId_, true);
	SetDrawBright(255, 255, 255);
	//DrawExtendGraph(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, ResourceManager::GetInstance().Load(ResourceManager::SRC::TITLE_IMAGE_1).handleId_, true);
}
