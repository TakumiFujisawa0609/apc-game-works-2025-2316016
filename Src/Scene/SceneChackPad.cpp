#include "../Application.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/KeyConfig.h"
#include "../Manager/SceneManager.h"
#include "SceneChackPad.h"

SceneChackPad::SceneChackPad(void)
{
}

SceneChackPad::~SceneChackPad(void)
{
}

bool SceneChackPad::Init(void)
{
	return false;
}

void SceneChackPad::Update(void)
{
	KeyConfig& ins = KeyConfig::GetInstance();
    if(ins.IsTrgDown(KeyConfig::CONTROL_TYPE::PAD_SHOW_HIDE, KeyConfig::JOYPAD_NO::PAD1) || ins.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER,KeyConfig::JOYPAD_NO::PAD1)
		|| ins.IsTrgDown(KeyConfig::CONTROL_TYPE::CANCEL, KeyConfig::JOYPAD_NO::PAD1) || ins.IsTrgDown(KeyConfig::CONTROL_TYPE::OPEN_MENU, KeyConfig::JOYPAD_NO::PAD1))
    {
        SceneManager::GetInstance().PopScene();
        return;
	}
}

void SceneChackPad::Draw(void)
{
	DrawBackBox();
	DrawRotaGraph(Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y, 1.0f, 0.0f, ResourceManager::GetInstance().Load(ResourceManager::SRC::PAD_UI_IMG).handleId_, true);
}

void SceneChackPad::Load(void)
{
}
void SceneChackPad::DrawBackBox(void)
{
    //˜g‘g‚Ý‚ð•`‰æ
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
    DrawBox(BOX_MARGIN, BOX_MARGIN, Application::SCREEN_SIZE_X - BOX_MARGIN, Application::SCREEN_SIZE_Y - BOX_MARGIN, 0, true);
    DrawBoxAA(BOX_MARGIN, BOX_MARGIN, Application::SCREEN_SIZE_X - BOX_MARGIN, Application::SCREEN_SIZE_Y - BOX_MARGIN, 0xffffff, false, 3.0f);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}