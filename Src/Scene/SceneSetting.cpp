#include <string>
#include "../Application.h"
#include "../../Utility/Utility.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/KeyConfig.h"
#include "../../Manager/SoundManager.h"
#include "SceneSetting.h"

SceneSetting::SceneSetting(void)
{
	frameCount_ = 0;
    type_ = TYPE::MAX;
    stateChange_[STATE::CHOOSE] = std::bind(&SceneSetting::ChooseChange, this);
    stateChange_[STATE::OTHER] = std::bind(&SceneSetting::OtherChange, this);
    seVolume_ = SoundManager::GetInstance().GetVolume(SoundManager::SOUND_TYPE::SE);
    bgmVolume_ = SoundManager::GetInstance().GetVolume(SoundManager::SOUND_TYPE::BGM);
	screenType_ = GetWindowModeFlag() ? SCREEN_MODE_TYPE::WINDOW_SCREEN : SCREEN_MODE_TYPE::FULL_SCREEN;
    ChangeState(STATE::CHOOSE);
}

SceneSetting::~SceneSetting(void)
{
}

bool SceneSetting::Init(void)
{
    return true;
}

void SceneSetting::Update(void)
{
	frameCount_++;
    stateUpdate_();
}

void SceneSetting::Draw(void)
{
    ResourceManager& resIns = ResourceManager::GetInstance();
	DrawBackBox();

    std::string str = "";
    int x = BOX_MARGIN + STRING_MARGIN;
    int startY = BOX_MARGIN + STRING_MARGIN;
    int y = startY;
    int b = 255 - static_cast<int>(abs(sin(Utility::Deg2RadF(static_cast<float>(frameCount_)))) * 255);
    if (state_ == STATE::OTHER)
    {
        b = 255;
    }
    if (type_ == TYPE::BGM_VOLUME)
    {
        SetDrawBright(255, 255, b);
    }
    DrawGraph(x, y, resIns.Load(ResourceManager::SRC::BGM_STR).handleId_, true);
    y += INTERVAL_Y + IMAGE_SIZE_Y;
    SetDrawBright(255, 255, 255);
    if (type_ == TYPE::SE_VOLUME)
    {
        SetDrawBright(255, 255, b);
    }
    DrawGraph(x, y, resIns.Load(ResourceManager::SRC::SE_STR).handleId_, true);
    y += INTERVAL_Y + IMAGE_SIZE_Y;
    SetDrawBright(255, 255, 255);
    if (type_ == TYPE::SCREEN_MODE)
    {
        SetDrawBright(255, 255, b);
    }
    DrawGraph(x, y, resIns.Load(ResourceManager::SRC::SCREEN_MODE_STR).handleId_, true);
    y += INTERVAL_Y + IMAGE_SIZE_Y;
    SetDrawBright(255, 255, 255);
    if (type_ == TYPE::END)
    {
        SetDrawBright(255, 255, b);
    }
    DrawGraph(x, y, resIns.Load(ResourceManager::SRC::RETURN_MENU_STR).handleId_, true);
    y += INTERVAL_Y + IMAGE_SIZE_Y;
    SetDrawBright(255, 255, 255);

    //音量を描画
    x = Application::SCREEN_HALF_X;
    y = BOX_MARGIN + STRING_MARGIN;
    Utility::DrawStringPlace("BGM 音量: " + std::to_string(static_cast<int>(bgmVolume_ * 100.0f)), Application::SCREEN_SIZE_X - BOX_MARGIN, y, 0xffffff, Utility::STRING_PLACE::RIGHT);
    DrawBox(x, y, x + GAGE_WIDTH, y + IMAGE_SIZE_Y, 0x000000, true);
    DrawBox(x, y, x + static_cast<int>(GAGE_WIDTH * bgmVolume_), y + IMAGE_SIZE_Y, 0x00ff00, true);
    if (type_ == TYPE::BGM_VOLUME && state_ == STATE::OTHER)
    {
        int perx = Application::SCREEN_HALF_X + static_cast<int>(GAGE_WIDTH * bgmVolume_);
        DrawLine(perx, y, perx, y + IMAGE_SIZE_Y, 0xffff00, 5);
    }
    y += INTERVAL_Y + IMAGE_SIZE_Y;
    Utility::DrawStringPlace("SE 音量: " + std::to_string(static_cast<int>(seVolume_ * 100.0f)), Application::SCREEN_SIZE_X - BOX_MARGIN, y, 0xffffff, Utility::STRING_PLACE::RIGHT);
    DrawBox(x, y, x + GAGE_WIDTH, y + IMAGE_SIZE_Y, 0x000000, true);
    DrawBox(x, y, x + static_cast<int>(GAGE_WIDTH * seVolume_), y + IMAGE_SIZE_Y, 0x00ff00, true);
    if (type_ == TYPE::SE_VOLUME && state_ == STATE::OTHER)
    {
        int perx = Application::SCREEN_HALF_X + static_cast<int>(GAGE_WIDTH * seVolume_);
        DrawLine(perx, y, perx, y + IMAGE_SIZE_Y, 0xffff00, 5);
    }
    //スクリーンモードを描画
    y += INTERVAL_Y + IMAGE_SIZE_Y;
    if (type_ == TYPE::SCREEN_MODE && screenType_ == selectScreenType_ && state_ == STATE::OTHER)
    {
        SetDrawBright(255, 255, 0);
    }
    DrawGraph(x, y, resIns.Load(screenType_ == SCREEN_MODE_TYPE::FULL_SCREEN ? ResourceManager::SRC::FULL_SCREEN_STR : ResourceManager::SRC::WINDOW_SCREEN_STR).handleId_, true);
    SetDrawBright(255, 255, 255);
    if (type_ == TYPE::SCREEN_MODE && state_ == STATE::OTHER)
    {
        y += INTERVAL_Y + IMAGE_SIZE_Y;
        if (screenType_ != selectScreenType_)
        {
            SetDrawBright(255, 255, 0);
        }
        DrawGraph(x, y, resIns.Load(screenType_ == SCREEN_MODE_TYPE::FULL_SCREEN ? ResourceManager::SRC::WINDOW_SCREEN_STR : ResourceManager::SRC::FULL_SCREEN_STR).handleId_, true);
        SetDrawBright(255, 255, 255);
    }
}

void SceneSetting::ChangeState(STATE state)
{
    state_ = state;
    stateChange_[state]();
}

void SceneSetting::ChooseChange(void)
{
    stateUpdate_ = std::bind(&SceneSetting::ChooseUpdate, this);
}

void SceneSetting::OtherChange(void)
{
	selectScreenType_ = screenType_;
    stateUpdate_ = std::bind(&SceneSetting::OtherUpdate, this);
}

void SceneSetting::ChooseUpdate(void)
{
    auto& ins = KeyConfig::GetInstance();
    int typeNum = static_cast<int>(type_);
    int typeMaxNum = static_cast<int>(TYPE::MAX);
    if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_DOWN, KeyConfig::JOYPAD_NO::PAD1))
    {
        typeNum++;
    }
    if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_UP, KeyConfig::JOYPAD_NO::PAD1))
    {
        typeNum--;
    }
    type_ = static_cast<TYPE>((typeNum + typeMaxNum) % typeMaxNum);
    if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
    {
        ChangeState(STATE::OTHER);
    }
}

void SceneSetting::OtherUpdate(void)
{
    auto& ins = KeyConfig::GetInstance();
    switch (type_)
    {
    case SceneSetting::TYPE::BGM_VOLUME:
		//音量調整
        if (ins.IsNew(KeyConfig::CONTROL_TYPE::SELECT_RIGHT, KeyConfig::JOYPAD_NO::PAD1))
        {
            bgmVolume_ += MOVE_SPEED;
        }
        if (ins.IsNew(KeyConfig::CONTROL_TYPE::SELECT_LEFT, KeyConfig::JOYPAD_NO::PAD1))
        {
            bgmVolume_ -= MOVE_SPEED;
        }
		//音量の範囲制限
		bgmVolume_ = std::max(0.0f, std::min(1.0f, bgmVolume_));
		//音量変更
        SoundManager::GetInstance().ChangeVolume(SoundManager::SOUND_TYPE::BGM, bgmVolume_);
        break;
    case SceneSetting::TYPE::SE_VOLUME:
		//音量調整
        if (ins.IsNew(KeyConfig::CONTROL_TYPE::SELECT_RIGHT, KeyConfig::JOYPAD_NO::PAD1))
        {
            seVolume_ += MOVE_SPEED;
        }
        if (ins.IsNew(KeyConfig::CONTROL_TYPE::SELECT_LEFT, KeyConfig::JOYPAD_NO::PAD1))
        {
            seVolume_ -= MOVE_SPEED;
        }
		//音量の範囲制限
		seVolume_ = std::max(0.0f, std::min(1.0f, seVolume_));
		//音量変更
        SoundManager::GetInstance().ChangeVolume(SoundManager::SOUND_TYPE::SE, seVolume_);
        break;
    case SceneSetting::TYPE::SCREEN_MODE:
		//スクリーンモード選択変更
        if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_UP, KeyConfig::JOYPAD_NO::PAD1) || ins.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_DOWN, KeyConfig::JOYPAD_NO::PAD1))
        {
			selectScreenType_ = (selectScreenType_ == SCREEN_MODE_TYPE::FULL_SCREEN) ? SCREEN_MODE_TYPE::WINDOW_SCREEN : SCREEN_MODE_TYPE::FULL_SCREEN;
        }
        break;
    case SceneSetting::TYPE::END:
        SceneManager::GetInstance().PopScene();
        return;
        break;
    case SceneSetting::TYPE::MAX:
        break;
    default:
        break;
    }

    if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
    {
        if (type_ == TYPE::SCREEN_MODE && selectScreenType_ != screenType_)
        {
            ChangeWindowMode(selectScreenType_ == SCREEN_MODE_TYPE::WINDOW_SCREEN);
			screenType_ = selectScreenType_;
        }
        ChangeState(STATE::CHOOSE);
    }
}

void SceneSetting::DrawBackBox(void)
{
    //枠組みを描画
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
    DrawBox(BOX_MARGIN, BOX_MARGIN, Application::SCREEN_SIZE_X - BOX_MARGIN, Application::SCREEN_SIZE_Y - BOX_MARGIN, 0, true);
    DrawBoxAA(BOX_MARGIN, BOX_MARGIN, Application::SCREEN_SIZE_X - BOX_MARGIN, Application::SCREEN_SIZE_Y - BOX_MARGIN, 0xffffff, false, 3.0f);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
