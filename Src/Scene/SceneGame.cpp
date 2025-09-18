#include<DxLib.h>
#include"../Application.h"
#include"../Manager/SceneManager.h"
#include"../Manager/ResourceManager.h"
#include"../Manager/SoundManager.h"
#include"../Manager/InputManager.h"
#include"../Manager/KeyConfig.h"
#include"../Manager/Camera.h"
#include "../Object/Player/PlayerBase.h"
#include "../Object/SkyDome/SkyDome.h"
#include"SceneGame.h"
#include "../Utility/Utility.h"

SceneGame::SceneGame(void)
{
}

SceneGame::~SceneGame(void)
{

}


//初期化処理(初回の1度のみ実行される)
bool SceneGame::Init(void)
{
	SceneBase::Init();
	//プレイヤー生成
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		players_[i] = std::make_unique<PlayerBase>(i);
		players_[i]->Init();
	}
	auto& cam = SceneManager::GetInstance().GetCamera();
	cam.ChangeMode(Camera::MODE::TWO_TARGET);
	cam.SetFollow(&players_[0]->GetTransform(), &players_[1]->GetTransform());
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Init();
	return true;
}

//更新処理
void SceneGame::Update(void)
{
	skyDome_->Update();
	//InputManager& ins = InputManager::GetInstance();
	KeyConfig& ins = KeyConfig::GetInstance();
	for (int i = 0;i < MAX_PLAYER;i++)
	{
		players_[i]->Update();
	}
}

//描画処理
void SceneGame::Draw(void)
{


	skyDome_->Draw();
	for (int i = 0;i < MAX_PLAYER;i++)
	{
		players_[i]->Draw();
	}
	DrawLine3D(players_[0]->GetTransform().pos, players_[1]->GetTransform().pos, GetColor(255, 0, 255));
	DebugDraw();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawCube3D(PlayerBase::MOVE_LIMIT_MIN, PlayerBase::MOVE_LIMIT_MAX, GetColor(255, 255, 255), GetColor(0, 0, 255), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SceneGame::DebugDraw(void)
{
	VECTOR min = PlayerBase::MOVE_LIMIT_MIN;
	VECTOR max = PlayerBase::MOVE_LIMIT_MAX;
	std::vector<VECTOR> vertexes;
	vertexes.push_back(VGet(min.x, min.y, min.z));
	vertexes.push_back(VGet(min.x, min.y, max.z));
	vertexes.push_back(VGet(max.x, min.y, min.z));
	vertexes.push_back(VGet(max.x, min.y, max.z));
	vertexes.push_back(VGet(min.x, max.y, min.z));
	vertexes.push_back(VGet(min.x, max.y, max.z));
	vertexes.push_back(VGet(max.x, max.y, min.z));
	vertexes.push_back(VGet(max.x, max.y, max.z));
	for (auto& vertex : vertexes)
	{
		DrawSphere3D(vertex, 16.0f, 32, 0xff00ff, 0xff00ff, true);
	}
	for (int i = 0; i < 4; i++)
	{
		DrawLine3D(vertexes[2 * i], vertexes[2 * i + 1], 0x0000ff);
		DrawLine3D(vertexes[i], vertexes[i + 4], 0x0000ff);
	}
	DrawLine3D(vertexes[0], vertexes[2], 0x0000ff);
	DrawLine3D(vertexes[1], vertexes[3], 0x0000ff);
	DrawLine3D(vertexes[4], vertexes[6], 0x0000ff);
	DrawLine3D(vertexes[5], vertexes[7], 0x0000ff);
}


