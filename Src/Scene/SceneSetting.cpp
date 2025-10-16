#include <string>
#include "../Application.h"
#include "../../Utility/Utility.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/KeyConfig.h"
#include "../../Manager/SoundManager.h"
#include "SceneSetting.h"

SceneSetting::SceneSetting(void)
{
    type_ = TYPE::MAX;
    stateChange_[STATE::CHOOSE] = std::bind(&SceneSetting::ChooseChange, this);
    stateChange_[STATE::OTHER] = std::bind(&SceneSetting::OtherChange, this);
    seVolume_ = SoundManager::GetInstance().GetVolume(SoundManager::SOUND_TYPE::SE);
    bgmVolume_ = SoundManager::GetInstance().GetVolume(SoundManager::SOUND_TYPE::BGM);
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
    stateUpdate_();
}

void SceneSetting::Draw(void)
{
    //枠組みを描画
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
    DrawBox(MARGIN, MARGIN, Application::SCREEN_SIZE_X - MARGIN, Application::SCREEN_SIZE_Y - MARGIN, 0, true);
    DrawBoxAA(MARGIN, MARGIN, Application::SCREEN_SIZE_X - MARGIN, Application::SCREEN_SIZE_Y - MARGIN, 0xffffff, false, 3.0f);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    std::string str = "";
    str = (type_ == TYPE::BGM_VOLUME ? "->" : "");
    str +="BGM 音量";
    Utility::DrawStringPlace(str,MARGIN + 30, MARGIN + 30, 0xffffff, Utility::STRING_PLACE::LEFT);
    str = (type_ == TYPE::SE_VOLUME ? "->" : "");
    str += "SE 音量";
    Utility::DrawStringPlace(str,MARGIN + 30, MARGIN + 60, 0xffffff, Utility::STRING_PLACE::LEFT);
    str = (type_ == TYPE::FULL_SCREEN ? "->" : "");
    str += "スクリーンモード";
    Utility::DrawStringPlace(str,MARGIN + 30, MARGIN + 90, 0xffffff, Utility::STRING_PLACE::LEFT);
    str = (type_ == TYPE::END ? "->" : "");
    str += "メニューに戻る";
    Utility::DrawStringPlace(str,MARGIN + 30, MARGIN + 120, 0xffffff, Utility::STRING_PLACE::LEFT);
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
        if (ins.IsNew(KeyConfig::CONTROL_TYPE::SELECT_RIGHT, KeyConfig::JOYPAD_NO::PAD1))
        {
            bgmVolume_ += MOVE_SPEED;
        }
        if (ins.IsNew(KeyConfig::CONTROL_TYPE::SELECT_LEFT, KeyConfig::JOYPAD_NO::PAD1))
        {
            bgmVolume_ -= MOVE_SPEED;
        }
        break;
    case SceneSetting::TYPE::SE_VOLUME:
        if (ins.IsNew(KeyConfig::CONTROL_TYPE::SELECT_RIGHT, KeyConfig::JOYPAD_NO::PAD1))
        {
            seVolume_ += MOVE_SPEED;
        }
        if (ins.IsNew(KeyConfig::CONTROL_TYPE::SELECT_LEFT, KeyConfig::JOYPAD_NO::PAD1))
        {
            seVolume_ -= MOVE_SPEED;
        }
        break;
    case SceneSetting::TYPE::FULL_SCREEN:
        break;
    case SceneSetting::TYPE::END:
        SceneManager::GetInstance().PopScene();
        break;
    case SceneSetting::TYPE::MAX:
        break;
    default:
        break;
    }

    if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
    {
        ChangeState(STATE::CHOOSE);
    }
}
