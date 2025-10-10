#include<DxLib.h>
#include"SceneManager.h"
#include "Camera.h"
#include"../Scene/SceneBase.h"
#include"../Common/Fader.h"
#include"../Scene/SceneTitle.h"
#include"../Scene/SceneGame.h"
#include"../Scene/SceneGameOver.h"
#include"../Scene/SceneGameClear.h"
#include"../Scene/SceneSelect.h"

SceneManager* SceneManager::instance_ = nullptr;


//初期化処理(初回の1度のみ実行される)
SceneManager::SceneManager(void)
{
	sceneID_ = SCENE_ID::NONE;
	nextSceneID_ = SCENE_ID::NONE;
	nowPushSpace = 0;
	oldPushSpace = 0;
	isSceneChanging_ = false;
	fader_ = nullptr;
	scene_ = nullptr;
	deltaTime_ = 0.0f;
}

SceneManager::~SceneManager(void)
{

}

bool SceneManager::Init(void)
{
	camera_ = std::make_unique<Camera>(0);
	sceneID_ = SCENE_ID::NONE;
	nextSceneID_ = SCENE_ID::TITLE;
	oldPushSpace = nowPushSpace;
	nowPushSpace = CheckHitKey(KEY_INPUT_SPACE);
	fader_ = std::make_unique<Fader>();
	fader_->Init();
	fader_->SetFade(Fader::STATE::FADE_IN);
	isSceneChanging_ = true;
	//インスタンス生成
	DoChangeScene();
	// デルタタイム
	deltaTime_ = 1.0f / 60.0f;
	Init3D();
	return true;
}

void SceneManager::Init3D(void)
{

	// Zバッファを有効にする
	SetUseZBuffer3D(true);

	// Zバッファへの書き込みを有効にする
	SetWriteZBuffer3D(true);

	// バックカリングを有効にする
	SetUseBackCulling(true);

	// ライトの設定
	SetUseLighting(false);
}

//更新処理
void SceneManager::Update(void)
{

	oldPushSpace = nowPushSpace;
	nowPushSpace = CheckHitKey(KEY_INPUT_SPACE);


	fader_->Update();
	//フェードの状態確認
	if (isSceneChanging_ == true)
	{
		//フェード中
		Fade();
	}
	//フェード終了
	else
	{
		scene_->Update();
		if (sceneID_ == SCENE_ID::TITLE)
		{
		}
	}

}

//描画処理
void SceneManager::Draw(void)
{
	ClearDrawScreen(); // 画面クリア
	camera_->SetBeforeDraw();
	scene_->Draw();
	fader_->Draw();

}

//解放処理(終了時の１度のみ実行)
void SceneManager::Destroy(void)
{
	ReleaseScene();
	fader_ = nullptr;

	delete instance_;               //インスタンスを削除
	instance_ = nullptr;            //インスタンス格納領域を初期化
}

float SceneManager::GetDeltaTime(void) const
{
	return deltaTime_;
}

//シーン切り替えを依頼する関数
void SceneManager::ChangeScene(SCENE_ID nextID, bool isToFade)
{
	nextSceneID_ = nextID;


	if (isToFade)
	{
		fader_->SetFade(Fader::STATE::FADE_OUT);
		isSceneChanging_ = true; 
	}
	else
	{
		DoChangeScene();
	}
}


//シーンを切り替える
void SceneManager::DoChangeScene(void)
{
	//現在のシーンの解放
	ReleaseScene();

	sceneID_ = nextSceneID_;

	switch (sceneID_)
	{
	case SCENE_ID::TITLE:
		scene_ = std::make_unique<SceneTitle>();
		break;
	case SCENE_ID::GAME:
		scene_ = std::make_unique<SceneGame>();
		break;
	case SCENE_ID::SELECT:
		scene_ = std::make_unique<SceneSelect>();
		break;
	case SCENE_ID::GAMEOVER:
		scene_ = std::make_unique<SceneGameOver>();
		break;
	case SCENE_ID::GAMECLEAR:
		scene_ = std::make_unique<SceneGameClear>();
		break;
	}
	nextSceneID_ = SCENE_ID::NONE;
	scene_->Init();
}

void SceneManager::Fade(void)
{
	//現在のフェード設定を取得
	Fader::STATE fState = fader_->GetState();

	switch (fState)
	{
	case Fader::STATE::FADE_OUT:
		if (fader_->IsEnd() == true)
		{
			//シーン切り替え
			DoChangeScene();

			//フェードで明るくしていく
			fader_->SetFade(Fader::STATE::FADE_IN);
		}
		
		break;

	case Fader::STATE::FADE_IN:
		if (fader_->IsEnd() == true)
		{
			//フェード終了
			fader_->SetFade(Fader::STATE::NONE);

			//シーン切り替えの終了
			isSceneChanging_ = false;
		}


		break;
	}
}
//指定したシーンの解放
void SceneManager::ReleaseScene(void)
{
	if (scene_ != nullptr)
	{
		scene_ = nullptr;
	}
}

void SceneManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new SceneManager();
	}
	instance_->Init();
}

SceneManager& SceneManager::GetInstance(void)
{
	return *instance_;
}
