#include "../Manager/KeyConfig.h"
#include "../Manager/SceneManager.h"
#include "SceneGameOver.h"

SceneGameOver::SceneGameOver(void)
{
}

SceneGameOver::~SceneGameOver(void)
{
}

bool SceneGameOver::Init(void)
{
    return true;
}

void SceneGameOver::Update(void)
{
    //決定キーを押されたらタイトルに戻る
    if (KeyConfig::GetInstance().IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
    {
        SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE, true);
    }
}

void SceneGameOver::Draw(void)
{
    DrawString(0, 0, "ゲームオーバーシーン", 0xffffff);
}
