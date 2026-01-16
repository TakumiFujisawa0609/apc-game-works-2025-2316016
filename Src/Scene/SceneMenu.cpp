#include "../Application.h"
#include "../../Utility/Utility.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "SceneMenu.h"

SceneMenu::SceneMenu(void) : ins_(KeyConfig::GetInstance())
{
	type_ = static_cast<TYPE>(0);
	time_ = 0.0f;
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
	time_ += SceneManager::GetInstance().GetDeltaTime();
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
		//case SceneMenu::TYPE::KEY_CONFIG:
		//	break;
		case SceneMenu::TYPE::CHACK_CONTROL:
			SceneManager::GetInstance().PushScene(SceneManager::SCENE_ID::CHACK_PAD);
			break;
		case SceneMenu::TYPE::OPTION:
			SceneManager::GetInstance().PushScene(SceneManager::SCENE_ID::SETTING);
			break;
		case SceneMenu::TYPE::BACK_GAME:
			//ゲームシーンに戻る
			SceneManager::GetInstance().PopScene();
			return;
			break;
		case SceneMenu::TYPE::GO_TITLE:
			//タイトルに戻る
			SceneManager::GetInstance().JumpScene(SceneManager::SCENE_ID::TITLE);
			return;
			break;
		case SceneMenu::TYPE::MAX:
			break;
		default:
			break;
		}
	}
	if(ins_.IsTrgDown(KeyConfig::CONTROL_TYPE::CANCEL,KeyConfig::JOYPAD_NO::PAD1)|| ins_.IsTrgDown(KeyConfig::CONTROL_TYPE::OPEN_MENU,KeyConfig::JOYPAD_NO::PAD1))
	{
		//ゲームシーンに戻る
		SceneManager::GetInstance().PopScene();
	}
}

void SceneMenu::Draw(void)
{
	ResourceManager& ins = ResourceManager::GetInstance();
	//枠組みを描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(MARGIN, MARGIN, Application::SCREEN_SIZE_X - MARGIN, Application::SCREEN_SIZE_Y - MARGIN, 0xffffff, true);
	DrawBoxAA(MARGIN, MARGIN, Application::SCREEN_SIZE_X - MARGIN, Application::SCREEN_SIZE_Y - MARGIN, 0, false, 3.0f);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//選択画面を文字列で描画
	//std::string str = "";
	//str += type_ == TYPE::KEY_CONFIG ? "->" : "";
	//str += "キー設定 \n";
	//str += type_ == TYPE::OPTION ? "->" : "";
	//str += "設定 \n";
	//str += type_ == TYPE::BACK_GAME ? "->" : "";
	//str += "ゲームに戻る \n";
	//str += type_ == TYPE::GO_TITLE ? "->" : "";
	//str += "タイトルに戻る \n";
	//Utility::DrawStringPlace(str, Application::SCREEN_HALF_X / 2, Application::SCREEN_HALF_Y / 2, 0, Utility::STRING_PLACE::LEFT);
	
	//選択肢を画像で描画
	SetDrawBright(255, 255, type_ == TYPE::CHACK_CONTROL ? 0: 255);
	DrawGraph(START_POS_X + (type_ == TYPE::CHACK_CONTROL ? abs(sinf(time_ * SELECT_SHAKE_POWER) * SELECT_SHAKE_WIDTH) : 0), START_POS_Y + INTERVAL_Y * static_cast<int>(TYPE::CHACK_CONTROL) + IMAGE_SIZE_Y * DrawScale * static_cast<int>(TYPE::CHACK_CONTROL), ins.Load(ResourceManager::SRC::CHACK_CONTROL_STR).handleId_, true);
	SetDrawBright(255, 255, type_ == TYPE::OPTION ? 0: 255);
	DrawGraph(START_POS_X + (type_ == TYPE::OPTION ? abs(sinf(time_ * SELECT_SHAKE_POWER) * SELECT_SHAKE_WIDTH) : 0), START_POS_Y + INTERVAL_Y * static_cast<int>(TYPE::OPTION) + IMAGE_SIZE_Y * DrawScale * static_cast<int>(TYPE::OPTION), ins.Load(ResourceManager::SRC::SETTING_STR).handleId_, true);
	SetDrawBright(255, 255, type_ == TYPE::BACK_GAME ? 0: 255);
	DrawGraph(START_POS_X + (type_ == TYPE::BACK_GAME ? abs(sinf(time_ * SELECT_SHAKE_POWER) * SELECT_SHAKE_WIDTH) : 0), START_POS_Y + INTERVAL_Y * static_cast<int>(TYPE::BACK_GAME) + IMAGE_SIZE_Y * DrawScale * static_cast<int>(TYPE::BACK_GAME), ins.Load(ResourceManager::SRC::RETURN_GAME_STR).handleId_, true);
	SetDrawBright(255, 255, type_ == TYPE::GO_TITLE ? 0: 255);
	DrawGraph(START_POS_X + (type_ == TYPE::GO_TITLE ? abs(sinf(time_ * SELECT_SHAKE_POWER) * SELECT_SHAKE_WIDTH) : 0), START_POS_Y + INTERVAL_Y * static_cast<int>(TYPE::GO_TITLE) + IMAGE_SIZE_Y * DrawScale * static_cast<int>(TYPE::GO_TITLE), ins.Load(ResourceManager::SRC::GO_TITLE_STR).handleId_, true);
	SetDrawBright(255, 255, 255);
}
