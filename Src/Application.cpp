#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "Manager/InputManager.h"
#include "Manager/KeyConfig.h"
#include "Manager/ResourceManager.h"
#include "Manager/SceneManager.h"
#include "Manager/SoundManager.h"
#include "Manager/DataBank.h"
#include "Manager/KeyMap.h"
#include "Common/FpsController.h"
#include "Application.h"

Application* Application::instance_ = nullptr;

const std::string Application::PATH_IMAGE = "Data/Image/";
const std::string Application::PATH_MODEL = "Data/Model/";
const std::string Application::PATH_EFFECT = "Data/Effect/";
const std::string Application::PATH_SOUND_BGM = "Data/Sound/BGM/";
const std::string Application::PATH_SOUND_SE = "Data/Sound/SE/";
//const std::string Application::PATH_FONT = "Data/Font/";
//const std::string Application::PATH_TEXT = "Data/Text/";
//const std::string Application::PATH_JSON = "Data/JSON/";
//const std::string Application::PATH_CSV = "Data/CSV/";
const std::string Application::PATH_SHADER = "Data/Shader/";

void Application::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Application();
	}
	instance_->Init();
}

Application& Application::GetInstance(void)
{
	return *instance_;
}

void Application::Init(void)
{

	// アプリケーションの初期設定
	SetWindowText("RJS : Roll Jump Shot");

	// ウィンドウサイズ
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	ChangeWindowMode(true);
	fpsController_ = std::make_unique<FpsController>(60.0f);

	// DxLibの初期化
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	isInitFail_ = false;
	if (DxLib_Init() == -1)
	{
		isInitFail_ = true;
		return;
	}
	// Effekseerの初期化
	InitEffekseer();
	// キー制御初期化
	SetUseDirectInputFlag(true);

	// リソース管理初期化
	ResourceManager::CreateInstance();	

	//入力管理の初期化
	KeyConfig::CreateInstance();

	//サウンド関係の初期化
	SoundManager::CreateInstance();

	// シーン管理初期化
	SceneManager::CreateInstance();

	DataBank::CreateInstance();

	KeyMap::GetInstance();
}

void Application::Run(void)
{
	LONGLONG time = GetNowHiPerformanceCount();

	KeyConfig& keyConfig = KeyConfig::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();

	// ゲームループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

		keyConfig.Update();
		sceneManager.Update();

		sceneManager.Draw();

		fpsController_->Draw();
		ScreenFlip();

		fpsController_->Wait();
	}

}

void Application::Destroy(void)
{
	SceneManager::GetInstance().Destroy();
	SoundManager::GetInstance().Destroy();	
	KeyConfig::GetInstance().Destroy();
	ResourceManager::GetInstance().Destroy();
	DataBank::GetInstance().Destroy();
	KeyMap::GetInstance().Destroy();
	// Effekseerを終了する。
	Effkseer_End();
	// DxLib終了
	if (DxLib_End() == -1)
	{
		isReleaseFail_ = true;
	}

	delete instance_;
	instance_ = nullptr;
}

bool Application::IsInitFail(void) const
{
	return isInitFail_;
}

bool Application::IsReleaseFail(void) const
{
	return isReleaseFail_;
}

Application::Application(void)
{
	isInitFail_ = false;
	isReleaseFail_ = false;
}

Application::~Application(void)
{

}
void Application::InitEffekseer(void)
{
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
	}

	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
}
