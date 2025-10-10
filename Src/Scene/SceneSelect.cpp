#include "../Application.h"
#include "../../Manager/DataBank.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "Select/Control.h"
#include "SceneSelect.h"

SceneSelect::SceneSelect(void)
{
	changeSelect_[SELECT::CONTROL_TYPE] = std::bind(&SceneSelect::ChangeSelectControlType, this);
	changeSelect_[SELECT::STAGE_SELECT] = std::bind(&SceneSelect::ChangeSelectStageSelect, this);
	ChangeSelect(SELECT::CONTROL_TYPE);
	control_ = std::make_unique<Control>();
}

SceneSelect::~SceneSelect(void)
{
}

bool SceneSelect::Init(void)
{
    return false;
}

void SceneSelect::Update(void)
{
	updateSelect_();
}

void SceneSelect::Draw(void)
{
	drawSelect_();
}

void SceneSelect::ChangeSelect(SELECT select)
{
	select_ = select;
	changeSelect_[select]();
}

void SceneSelect::ChangeSelectControlType(void)
{
	updateSelect_ = std::bind(&SceneSelect::UpdateSelectControlType, this);
	drawSelect_ = std::bind(&SceneSelect::DrawSelectControlType, this);
}

void SceneSelect::ChangeSelectStageSelect(void)
{
	updateSelect_ = std::bind(&SceneSelect::UpdateSelectStageSelect, this);
	drawSelect_ = std::bind(&SceneSelect::DrawSelectStageSelect, this);
}

void SceneSelect::UpdateSelectControlType(void)
{
	control_->Update();
	if (control_->IsEnd())
	{
		ChangeSelect(SELECT::STAGE_SELECT);
	}
}

void SceneSelect::UpdateSelectStageSelect(void)
{
	if (KeyConfig::GetInstance().IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME, true);
	}
}

void SceneSelect::DrawSelectControlType(void)
{
	control_->Draw();
	DrawEnterCancelKey();
}

void SceneSelect::DrawSelectStageSelect(void)
{
	DrawString(100, 100, "ステージセレクト", GetColor(255, 255, 255));
}

void SceneSelect::DrawEnterCancelKey(void)
{
	KeyConfig& ins = KeyConfig::GetInstance();
	std::string str;
	str = "";
	for (auto btn : ins.GetControlBTN(KeyConfig::CONTROL_TYPE::ENTER))
	{
		str += Utility::GetBtnName(btn) + " , ";
	}
	str += " :決定 ,";
	for (auto btn : ins.GetControlBTN(KeyConfig::CONTROL_TYPE::CANCEL))
	{
		str += Utility::GetBtnName(btn) + " , ";
	}
	str += " :キャンセル";
	Utility::DrawStringPlace(str, Application::SCREEN_SIZE_X - 10, Application::SCREEN_SIZE_Y - 20, 0xffffff, Utility::STRING_PLACE::RIGHT);
}

void SceneSelect::DrawEnterCancelKeyTemp(void)
{
	KeyConfig& ins = KeyConfig::GetInstance();
	std::string str;
	str = "";
	for (auto btn : ins.GetControlBTN(KeyConfig::CONTROL_TYPE::ENTER_TEMP))
	{
		str += Utility::GetBtnName(btn) + " , ";
	}
	str += " :決定 ,";
	for (auto btn : ins.GetControlBTN(KeyConfig::CONTROL_TYPE::CANCEL_TEMP))
	{
		str += Utility::GetBtnName(btn) + " , ";
	}
	str += " :キャンセル";
	Utility::DrawStringPlace(str, Application::SCREEN_SIZE_X - 10, Application::SCREEN_SIZE_Y - 20, 0xffffff, Utility::STRING_PLACE::RIGHT);
}
