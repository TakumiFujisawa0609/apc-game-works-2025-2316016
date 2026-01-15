#include <DxLib.h>
#include "../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(void)
{

	Resource res;
	// ボタン
	std::string buttonPath = Application::PATH_IMAGE + "Button/";
	res = Resource(Resource::TYPE::IMG, buttonPath + "A.png");
	resourcesMap_.emplace(SRC::BUTTON_A, std::move(res));
	res = Resource(Resource::TYPE::IMG, buttonPath + "B.png");
	resourcesMap_.emplace(SRC::BUTTON_B, std::move(res));
	res = Resource(Resource::TYPE::IMG, buttonPath + "X.png");
	resourcesMap_.emplace(SRC::BUTTON_X, std::move(res));
	res = Resource(Resource::TYPE::IMG, buttonPath + "Y.png");
	resourcesMap_.emplace(SRC::BUTTON_Y, std::move(res));
	res = Resource(Resource::TYPE::IMG, buttonPath + "LB.png");
	resourcesMap_.emplace(SRC::BUTTON_LB, std::move(res));
	res = Resource(Resource::TYPE::IMG, buttonPath + "RB.png");
	resourcesMap_.emplace(SRC::BUTTON_RB, std::move(res));
	res = Resource(Resource::TYPE::IMG, buttonPath + "LT.png");
	resourcesMap_.emplace(SRC::BUTTON_LT, std::move(res));
	res = Resource(Resource::TYPE::IMG, buttonPath + "RT.png");
	resourcesMap_.emplace(SRC::BUTTON_RT, std::move(res));
	res = Resource(Resource::TYPE::IMG, buttonPath + "Menu.png");
	resourcesMap_.emplace(SRC::BUTTON_SELECT, std::move(res));
	res = Resource(Resource::TYPE::IMG, buttonPath + "Start.png");
	resourcesMap_.emplace(SRC::BUTTON_START, std::move(res));
	res = Resource(Resource::TYPE::IMG, buttonPath + "UP.png");
	resourcesMap_.emplace(SRC::BUTTON_UP, std::move(res));
	res = Resource(Resource::TYPE::IMG, buttonPath + "DOWN.png");
	resourcesMap_.emplace(SRC::BUTTON_DOWN, std::move(res));
	res = Resource(Resource::TYPE::IMG, buttonPath + "LEFT.png");
	resourcesMap_.emplace(SRC::BUTTON_LEFT, std::move(res));
	res = Resource(Resource::TYPE::IMG, buttonPath + "RIGHT.png");
	resourcesMap_.emplace(SRC::BUTTON_RIGHT, std::move(res));

	//操作の文字
	std::string strPath = Application::PATH_IMAGE + "ControlType/";
	res = Resource(Resource::TYPE::IMG, strPath + "Enter.png");
	resourcesMap_.emplace(SRC::ENTER_STR, std::move(res));
	res = Resource(Resource::TYPE::IMG, strPath + "Cancel.png");
	resourcesMap_.emplace(SRC::CANCEL_STR, std::move(res));
	res = Resource(Resource::TYPE::IMG, strPath + "Avoid.png");
	resourcesMap_.emplace(SRC::AVOID_STR, std::move(res));
	res = Resource(Resource::TYPE::IMG, strPath + "Attack.png");
	resourcesMap_.emplace(SRC::ATTACK_STR, std::move(res));
	res = Resource(Resource::TYPE::IMG, strPath + "Jump.png");
	resourcesMap_.emplace(SRC::JUMP_STR, std::move(res));
	res = Resource(Resource::TYPE::IMG, strPath + "LockOn.png");
	resourcesMap_.emplace(SRC::LOCKON_STR, std::move(res));

	//メニュー用
	std::string menuPath = Application::PATH_IMAGE + "Menu/";
	res = Resource(Resource::TYPE::IMG, menuPath + "Setting.png");
	resourcesMap_.emplace(SRC::SETTING_STR, std::move(res));
	res = Resource(Resource::TYPE::IMG, menuPath + "GoTitle.png");
	resourcesMap_.emplace(SRC::GO_TITLE_STR, std::move(res));
	res = Resource(Resource::TYPE::IMG, menuPath + "ReturnGame.png");
	resourcesMap_.emplace(SRC::RETURN_GAME_STR, std::move(res));
	res = Resource(Resource::TYPE::IMG, menuPath + "BGM.png");
	resourcesMap_.emplace(SRC::BGM_STR, std::move(res));
	res = Resource(Resource::TYPE::IMG, menuPath + "SE.png");
	resourcesMap_.emplace(SRC::SE_STR, std::move(res));
	res = Resource(Resource::TYPE::IMG, menuPath + "ScreenMode.png");
	resourcesMap_.emplace(SRC::SCREEN_MODE_STR, std::move(res));
	res = Resource(Resource::TYPE::IMG, menuPath + "FullScreen.png");
	resourcesMap_.emplace(SRC::FULL_SCREEN_STR, std::move(res));
	res = Resource(Resource::TYPE::IMG, menuPath + "WindowScreen.png");
	resourcesMap_.emplace(SRC::WINDOW_SCREEN_STR, std::move(res));
	res = Resource(Resource::TYPE::IMG, menuPath + "ReturnMenu.png");
	resourcesMap_.emplace(SRC::RETURN_MENU_STR, std::move(res));


	//タイトル
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Title/Title.png");
	resourcesMap_.emplace(SRC::TITLE_IMAGE, std::move(res));

	//オブジェクト
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "SkyDome/SkyDome.mv1");
	resourcesMap_.emplace(SRC::SKY_DOME, std::move(res));
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Player.mv1");
	resourcesMap_.emplace(SRC::PLAYER, std::move(res));
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Shot/Shot.mv1");
	resourcesMap_.emplace(SRC::SHOT, std::move(res));
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Chicken/Chicken.mv1");
	resourcesMap_.emplace(SRC::CHICKIN, std::move(res));
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "SnowMan/SnowMan.mv1");
	resourcesMap_.emplace(SRC::SNOW_MAN, std::move(res));
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Dragon/Blue.mv1");
	resourcesMap_.emplace(SRC::DRAGON, std::move(res));
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage/Floor/Floor.mv1");
	resourcesMap_.emplace(SRC::FLOOR, std::move(res));
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Gate/Gate.mv1");
	resourcesMap_.emplace(SRC::GATE, std::move(res));

	//UI
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "UI/EnemyHP.png");
	resourcesMap_.emplace(SRC::ENEMY_HP_UI_IMG, std::move(res));
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "UI/EnemyHPMask.png");
	resourcesMap_.emplace(SRC::ENEMY_HP_UI_MASK_IMG, std::move(res));
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "UI/EnemyHPNormal.png");
	resourcesMap_.emplace(SRC::ENEMY_HP_UI_NORMAL_IMG, std::move(res));

	//テクスチャ
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Texture/Wave.png");
	resourcesMap_.emplace(SRC::WAVE_TEXTURE, std::move(res));
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Texture/Noise.png");
	resourcesMap_.emplace(SRC::NOISE, std::move(res));
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Texture/Texture1.png");
	resourcesMap_.emplace(SRC::TEXTURE_1, std::move(res));

	//エフェクト
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Hit.efkefc");
	resourcesMap_.emplace(SRC::HIT_EFFECT, std::move(res));
}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second->Release();
		delete p.second;
	}

	loadedMap_.clear();
}

void ResourceManager::Destroy(void)
{
	Release();
	resourcesMap_.clear();
	delete instance_;
}

Resource ResourceManager::Load(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return Resource();
	}
	Resource ret = *res;
	return *res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res->handleId_);
	res->duplicateModelIds_.push_back(duId);

	return duId;
}

ResourceManager::ResourceManager(void)
{
}

Resource* ResourceManager::_Load(SRC src)
{
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return lPair->second;
	}

	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// 登録されていない
		return nullptr;
	}
	//for (auto& pair : resourcesMap_)
	//{
	//	if (pair.first != src)
	//	{
	//		continue;
	//	}
	//	pair.second.Load();
	//	break;
	//}
	rPair->second.Load();

	// 念のためコピーコンストラクタ
	Resource* ret = new Resource(rPair->second);
	loadedMap_.emplace(src, ret);

	return ret;
}
