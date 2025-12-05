#include "../Application.h"
#include "../../Manager/DataBank.h"
#include "../../Manager/Camera.h"
#include "../../Manager/DrawTranslucentManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/CollisionManager.h"
#include "../Object/Player/GamePlayer.h"
#include "../Object/Stage/Stage.h"
#include "../Object/SkyDome/SkyDome.h"
#include "../Object/Gate/GateManager.h"
#include "SceneStageSelect.h"

SceneStageSelect::SceneStageSelect(void)
{

}

SceneStageSelect::~SceneStageSelect(void)
{
}

bool SceneStageSelect::Init(void)
{
	player_ = std::make_unique<GamePlayer>(0);
	player_->SetPos(Utility::VECTOR_ZERO);
	stage_ = std::make_unique<Stage>(player_->GetPointLight());
	skyDome_ = std::make_unique<SkyDome>();
	//float degInterval = Utility::ONE_TRACK_DEG / static_cast<float>(GATE_NUM);
	//for (int i = 0; i < GATE_NUM; i++)
	//{
	//	gate_[i] = std::make_unique<Gate>(SceneManager::SCENE_ID::GAME);
	//	gate_[i]->SetPos({sinf(Utility::Deg2RadF(degInterval) * i) * GATE_RADIUS,0.0f,cosf(Utility::Deg2RadF(degInterval) * i) * GATE_RADIUS });
	//	gate_[i]->SetDegY(degInterval * i);
	//}
	std::vector<SceneManager::SCENE_ID> nextId = {};
	nextId.push_back(SceneManager::SCENE_ID::GAME);
	gateManager_ = std::make_unique<GateManager>(nextId);
	//カメラ設定
	auto& cam = SceneManager::GetInstance().GetCamera();
	cam.SetFollow(player_->GetTransform().lock());
	cam.ChangeMode(Camera::MODE::FOLLOW);
	cam.SetPos(player_->GetTransform().lock()->pos);
	return true;
}

void SceneStageSelect::Update(void)
{
	skyDome_->Update();
	stage_->Update();
	player_->Update();
	gateManager_->Update();

	VECTOR pPos = player_->GetTransform().lock()->pos;
	pPos.y = 0.0f;
	auto dis = Utility::Distance(pPos, Utility::VECTOR_ZERO);
	if (dis > Stage::RADIUS)
	{
		VECTOR dir = VNorm(VSub(pPos, Utility::VECTOR_ZERO));
		VECTOR pos = VScale(dir, Stage::RADIUS);
		pos.y = player_->GetTransform().lock()->pos.y;
		player_->SetPos(pos);
	}
	CollisionManager::GetInstance().Update();
}

void SceneStageSelect::Draw(void)
{
	skyDome_->Draw();
	stage_->Draw();
	player_->Draw();
	gateManager_->Draw();
	DrawTranslucentManager::GetInstance().Draw();
}
