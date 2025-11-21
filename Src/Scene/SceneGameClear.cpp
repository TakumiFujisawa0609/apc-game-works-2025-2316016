#include "../Manager/Camera.h"
#include "../Manager/KeyConfig.h"
#include "../Manager/SceneManager.h"
#include "SceneGameClear.h"

SceneGameClear::SceneGameClear(void)
{
    auto& camera = SceneManager::GetInstance().GetCamera();
    camera.ChangeMode(Camera::MODE::FIXED_POINT);
}

SceneGameClear::~SceneGameClear(void)
{
}

bool SceneGameClear::Init(void)
{
	return false;
}

void SceneGameClear::Update(void)
{
    //決定キーを押したらタイトルに戻る
    if (KeyConfig::GetInstance().IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
    {
        SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE, true);
    }
}

void SceneGameClear::Draw(void)
{
    DrawString(0, 0, "ゲームクリアシーン", 0xffffff);
}
