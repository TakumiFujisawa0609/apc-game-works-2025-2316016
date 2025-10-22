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
    //枠組みを描画
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
    DrawBox(BOX_MARGIN, BOX_MARGIN, Application::SCREEN_SIZE_X - BOX_MARGIN, Application::SCREEN_SIZE_Y - BOX_MARGIN, 0, true);
    DrawBoxAA(BOX_MARGIN, BOX_MARGIN, Application::SCREEN_SIZE_X - BOX_MARGIN, Application::SCREEN_SIZE_Y - BOX_MARGIN, 0xffffff, false, 3.0f);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    std::string str = "";
    int x = BOX_MARGIN + STRING_MARGIN;
    int startY = BOX_MARGIN + STRING_MARGIN;
	int y = startY;
    if (type_ == TYPE::BGM_VOLUME)
    {
		SetDrawBright(255, 255 , 255 - abs(sin(Utility::Deg2RadF(frameCount_))) * 255);
    }
    DrawExtendGraph(x, y, x + IMAGE_SIZE_X, y + IMAGE_SIZE_Y, ResourceManager::GetInstance().Load(ResourceManager::SRC::BGM_STR).handleId_, true);
	y += INTERVAL_Y;
    SetDrawBright(255, 255, 255);
    if (type_ == TYPE::SE_VOLUME)
    {
        SetDrawBright(255, 255, 255 - abs(sin(Utility::Deg2RadF(frameCount_))) * 255);
    }
    DrawExtendGraph(x, y, x + IMAGE_SIZE_X, y + IMAGE_SIZE_Y, ResourceManager::GetInstance().Load(ResourceManager::SRC::SE_STR).handleId_, true);
    y += INTERVAL_Y;
    SetDrawBright(255, 255, 255);
    str = (type_ == TYPE::FULL_SCREEN ? "->" : "");
    str += "スクリーンモード";
    Utility::DrawStringPlace(str,BOX_MARGIN + 30, BOX_MARGIN + 90, 0xffffff, Utility::STRING_PLACE::LEFT);
    str = (type_ == TYPE::END ? "->" : "");
    str += "メニューに戻る";
    Utility::DrawStringPlace(str,BOX_MARGIN + 30, BOX_MARGIN + 120, 0xffffff, Utility::STRING_PLACE::LEFT);
	
    //音量を描画
	Utility::DrawStringPlace("BGM 音量: " + std::to_string(static_cast<int>(bgmVolume_ * 100.0f)), Application::SCREEN_SIZE_X - BOX_MARGIN, BOX_MARGIN + 30, 0xffffff, Utility::STRING_PLACE::RIGHT);
	DrawBox(Application::SCREEN_HALF_X, BOX_MARGIN + 25, Application::SCREEN_HALF_X + GAGE_WIDTH, BOX_MARGIN + 25 + GAGE_HEIGHT, 0x000000, true);
	DrawBox(Application::SCREEN_HALF_X, BOX_MARGIN + 25, Application::SCREEN_HALF_X + static_cast<int>(GAGE_WIDTH * bgmVolume_), BOX_MARGIN + 25 + GAGE_HEIGHT, 0x00ff00, true);
    if (type_ == TYPE::BGM_VOLUME && state_ == STATE::OTHER)
    {
        int x = Application::SCREEN_HALF_X + static_cast<int>(GAGE_WIDTH * bgmVolume_);
        DrawLine(x, BOX_MARGIN + 25, x, BOX_MARGIN + 25 + GAGE_HEIGHT, 0xffff00, 5);
    }
	Utility::DrawStringPlace("SE 音量: " + std::to_string(static_cast<int>(seVolume_ * 100.0f)), Application::SCREEN_SIZE_X - BOX_MARGIN, BOX_MARGIN + 60, 0xffffff, Utility::STRING_PLACE::RIGHT);
	DrawBox(Application::SCREEN_HALF_X, BOX_MARGIN + 55, Application::SCREEN_HALF_X + GAGE_WIDTH, BOX_MARGIN + 55 + GAGE_HEIGHT, 0x000000, true);
	DrawBox(Application::SCREEN_HALF_X, BOX_MARGIN + 55, Application::SCREEN_HALF_X + static_cast<int>(GAGE_WIDTH * seVolume_), BOX_MARGIN + 55 + GAGE_HEIGHT, 0x00ff00, true);
    if (type_ == TYPE::SE_VOLUME && state_ == STATE::OTHER)
    {
        int x = Application::SCREEN_HALF_X + static_cast<int>(GAGE_WIDTH * seVolume_);
        DrawLine(x, BOX_MARGIN + 55, x, BOX_MARGIN + 55 + GAGE_HEIGHT, 0xffff00, 5);
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
		bgmVolume_ = std::max(0.0f, std::min(1.0f, bgmVolume_));
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
		seVolume_ = std::max(0.0f, std::min(1.0f, seVolume_));
        break;
    case SceneSetting::TYPE::FULL_SCREEN:
        break;
    case SceneSetting::TYPE::END:
		SoundManager::GetInstance().ChangeVolume(SoundManager::SOUND_TYPE::SE, seVolume_);
		SoundManager::GetInstance().ChangeVolume(SoundManager::SOUND_TYPE::BGM, bgmVolume_);
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
