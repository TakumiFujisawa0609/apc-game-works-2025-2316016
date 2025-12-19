#include "../Application.h"
#include "../Manager/Camera.h"
#include "../Manager/KeyConfig.h"
#include "../Manager/SoundManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Object/Enemy/EnemyClear.h"
#include "../Object/Player/ClearPlayer.h"
#include "../Object/SkyDome/SkyDome.h"
#include "SceneGameClear.h"

SceneGameClear::SceneGameClear(void)
{
    auto& camera = SceneManager::GetInstance().GetCamera();
    camera.ChangeMode(Camera::MODE::FIXED_POINT);
}

SceneGameClear::~SceneGameClear(void)
{
	SoundManager::GetInstance().Stop(SoundManager::SRC::CLEAR_BGM);
}

bool SceneGameClear::Init(void)
{
    auto& camera = SceneManager::GetInstance().GetCamera();
    camera.ChangeMode(Camera::MODE::FIXED_POINT);
    camera.SetPos(CAMERA_POS);
    camera.SetTargetPos(CAMERA_TARGET_POS);
    camera.SetCameraUp(Utility::DIR_U);

    player_ = std::make_unique<ClearPlayer>(0);
    enemy_ = std::make_unique<EnemyClear>(player_->GetTransform());
    skyDome_ = std::make_unique<SkyDome>();
    skyDome_->SetColor(SKY_COL);

	SoundManager::GetInstance().Play(SoundManager::SRC::CLEAR_BGM, Sound::TIMES::LOOP);
	return false;
}

void SceneGameClear::Update(void)
{
    skyDome_->Update();
    player_->Update();
    enemy_->Update();
    //ƒQ[ƒ€ƒV[ƒ“‚Ö
    if (KeyConfig::GetInstance().IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
    {
        SoundManager::GetInstance().Play(SoundManager::SRC::ENTER_SOUND, Sound::TIMES::ONCE);
        SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE, true);
    }
}

void SceneGameClear::Draw(void)
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
	//SetDrawBright(255, 255, 255);
	//DrawExtendGraph(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, ResourceManager::GetInstance().Load(ResourceManager::SRC::TITLE_IMAGE_1).handleId_, true);
}
