#include <DxLib.h>
#include <cmath>
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/SoundManager.h"
#include "../Manager/KeyConfig.h"
#include "../Manager/Camera.h"
#include "../Manager/ResourceManager.h"
#include "../Object/Player/TitlePlayer.h"
#include "../Object/Enemy/EnemyTitle.h"
#include "../Object/SkyDome/SkyDome.h"
#include "../Object/UI/PushKeyUI.h"
#include "SceneTitle.h"


SceneTitle::SceneTitle(void)
{
}

SceneTitle::~SceneTitle(void)
{
	SoundManager::GetInstance().Stop(SoundManager::SRC::TITLE_BGM);
}

//初期化処理(初回の1度のみ実行される)
bool SceneTitle::Init(void)
{
	auto& camera = SceneManager::GetInstance().GetCamera();
	camera.ChangeMode(Camera::MODE::FIXED_POINT);
	camera.SetPos(CAMERA_POS);
	camera.SetTargetPos(CAMERA_TARGET_POS);
	camera.SetCameraUp(Utility::DIR_U);
	SoundManager::GetInstance().Play(SoundManager::SRC::TITLE_BGM, Sound::TIMES::LOOP);
	SetUseASyncLoadFlag(true);
	SoundManager::GetInstance().Load(SoundManager::SRC::GAME_BGM);
	SetUseASyncLoadFlag(false);

	player_ = std::make_unique<TitlePlayer>(0);
	enemy_ = std::make_unique<EnemyTitle>();
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->SetColor(SKY_COL);

	InitPushKeyUI();
	return true;
}

//更新処理
void SceneTitle::Update(void)
{
	pushKeyUI_->Update();
	skyDome_->Update();
	player_->Update();
	enemy_->Update();
	//ゲームシーンへ
	if (KeyConfig::GetInstance().IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER,KeyConfig::JOYPAD_NO::PAD1))
	{
		SoundManager::GetInstance().Play(SoundManager::SRC::ENTER_SOUND, Sound::TIMES::ONCE);
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME, true);
	}
	
}
//描画処理
void SceneTitle::Draw(void)
{
	skyDome_->Draw();
	VECTOR pos;
	VECTOR centerPos;
	VECTOR rot;
	VECTOR dist;
	centerPos = ConvScreenPosToWorldPos(VGet(Application::SCREEN_HALF_X, Application::SCREEN_SIZE_Y - MARGIN, 0.95f));
	pos = ConvScreenPosToWorldPos(VGet(Application::SCREEN_HALF_X / 2, Application::SCREEN_SIZE_Y - MARGIN, 0.95f));
	player_->SetPos(pos);
	dist = VSub(centerPos, pos);
	rot.y = atan2f(-dist.x, -dist.z);
	player_->SetRot(VGet(0.0f, rot.y, 0.0f));
	pos = ConvScreenPosToWorldPos(VGet(Application::SCREEN_SIZE_X - Application::SCREEN_HALF_X / 2, Application::SCREEN_SIZE_Y - MARGIN, 0.95f));
	enemy_->SetPos(pos);
	dist = VSub(centerPos, pos);
	rot.y = atan2f(-dist.x, -dist.z);
	enemy_->SetRot(VGet(0.0f, rot.y, 0.0f));
	enemy_->SetScl(ENEMY_SIZE);
	player_->Draw();
	enemy_->Draw();
	//DrawString(100, 100, "Title", GetColor(255, 255, 255));
	//SetDrawBright(255, 0, 0);
	//DrawExtendGraph(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, ResourceManager::GetInstance().Load(ResourceManager::SRC::TITLE_IMAGE_2).handleId_, true);
	DrawRotaGraph(Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y, 1.0f, 0.0f, ResourceManager::GetInstance().Load(ResourceManager::SRC::TITLE_IMAGE).handleId_, true);
	pushKeyUI_->Draw();
	//SetDrawBright(255, 255, 255);
	//DrawExtendGraph(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, ResourceManager::GetInstance().Load(ResourceManager::SRC::TITLE_IMAGE_1).handleId_, true);
}

void SceneTitle::Load(void)
{

}

void SceneTitle::InitPushKeyUI(void)
{
	pushKeyUI_ = std::make_unique<PushKeyUI>(FLOAT3(1.0f, 1.0f, 1.0f),PushKeyUI::TYPE::TITLE,VECTOR{1.0f,1.0f,1.0f},FLOAT2{PUSH_KEY_SIZE_X,PUSH_KEY_SIZE_Y});
	pushKeyInfo_.clear();
	auto& vertex = pushKeyInfo_.vertex;
	for (int i = 0; i < 4; i++)
	{
		VERTEX2DSHADER ver;
		ver.pos = VGet(Application::SCREEN_HALF_X - (PUSH_KEY_SIZE_X / 2) + ((i % 2) * (PUSH_KEY_SIZE_X)),Application::SCREEN_SIZE_Y - MARGIN + (i / 2)* (PUSH_KEY_SIZE_Y),0.0f);
		ver.dif = GetColorU8(255, 255, 255, 255);
		ver.rhw = 1.0f;
		ver.u = static_cast<float>(i % 2);
		ver.v = static_cast<float>(i / 2);
		ver.su = ver.pos.x / Application::SCREEN_SIZE_X;
		ver.sv = ver.pos.y / Application::SCREEN_SIZE_Y;
		ver.spc = GetColorU8(0, 0, 0, 0);
		vertex.push_back(ver);
	}
	pushKeyInfo_.Indices = { 0,1,2,1,3,2 };
	pushKeyUI_->SetPolygonInfo(pushKeyInfo_);
}
