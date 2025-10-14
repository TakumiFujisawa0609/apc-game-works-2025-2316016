#include "../Application.h"
#include "../../Manager/DataBank.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "SceneStageSelect.h"

SceneStageSelect::SceneStageSelect(void)
{

}

SceneStageSelect::~SceneStageSelect(void)
{
}

bool SceneStageSelect::Init(void)
{
    return false;
}

void SceneStageSelect::Update(void)
{
}

void SceneStageSelect::Draw(void)
{
	Utility::DrawStringPlace("ステージセレクト", Application::SCREEN_HALF_X, 0, 0xffffff, Utility::STRING_PLACE::CENTER);
	DrawEnterCancelKey();
}

void SceneStageSelect::DrawEnterCancelKey(void)
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

void SceneStageSelect::DrawEnterCancelKeyTemp(void)
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
