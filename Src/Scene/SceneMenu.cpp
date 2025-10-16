#include "../Application.h"
#include "../../Utility/Utility.h"
#include "../../Manager/SceneManager.h"
#include "SceneMenu.h"

SceneMenu::SceneMenu(void) : ins_(KeyConfig::GetInstance())
{
	type_ = static_cast<TYPE>(0);
}

SceneMenu::~SceneMenu(void)
{
}

bool SceneMenu::Init(void)
{
	return true;
}

void SceneMenu::Update(void)
{
	//上もしくわ下を押した時に選択を移動する
	if (ins_.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_DOWN, KeyConfig::JOYPAD_NO::PAD1))
	{
		type_ = static_cast<TYPE>((static_cast<int>(type_) + 1) % static_cast<int>(TYPE::MAX));
	}
	if (ins_.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_UP, KeyConfig::JOYPAD_NO::PAD1))
	{
		type_ = static_cast<TYPE>((static_cast<int>(type_) - 1 + static_cast<int>(TYPE::MAX)) % static_cast<int>(TYPE::MAX));
	}

	//決定キーが押されたら選択中のものに応じてシーンを変える
	if (ins_.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
	{
		switch (type_)
		{
		case SceneMenu::TYPE::KEY_CONFIG:
			break;
		case SceneMenu::TYPE::OPTION:
			break;
		case SceneMenu::TYPE::BACK_GAME:
			//ゲームシーンに戻る
			SceneManager::GetInstance().PopScene();
			break;
		case SceneMenu::TYPE::GO_TITLE:
			//タイトルに戻る
			SceneManager::GetInstance().JumpScene(SceneManager::SCENE_ID::TITLE);
			break;
		case SceneMenu::TYPE::MAX:
			break;
		default:
			break;
		}
	}
}

void SceneMenu::Draw(void)
{
	//枠組みを描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(MARGIN, MARGIN, Application::SCREEN_SIZE_X - MARGIN, Application::SCREEN_SIZE_Y - MARGIN, 0xffffff, true);
	DrawBoxAA(MARGIN, MARGIN, Application::SCREEN_SIZE_X - MARGIN, Application::SCREEN_SIZE_Y - MARGIN, 0, false, 3.0f);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//選択画面を文字列で描画
	std::string str = "";
	str += type_ == TYPE::KEY_CONFIG ? "->" : "";
	str += "キー設定 \n";
	str += type_ == TYPE::OPTION ? "->" : "";
	str += "設定 \n";
	str += type_ == TYPE::BACK_GAME ? "->" : "";
	str += "ゲームに戻る \n";
	str += type_ == TYPE::GO_TITLE ? "->" : "";
	str += "タイトルに戻る \n";
	Utility::DrawStringPlace(str, Application::SCREEN_HALF_X / 2, Application::SCREEN_HALF_Y / 2, 0, Utility::STRING_PLACE::LEFT);
}
