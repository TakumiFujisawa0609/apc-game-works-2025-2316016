#include<DxLib.h>
#include "../Utility/Utility.h"
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SoundManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/KeyConfig.h"
#include "../Manager/Camera.h"
#include "../Manager/DataBank.h"
#include "../Object/Player/PlayerBase.h"
#include "../Object/SkyDome/SkyDome.h"
#include "../Object/Player/PlayerShot.h"
#include "../Object/Enemy/EnemyBase.h"
#include "../Object/Enemy/Attack/AttackBase.h"
#include "../Object/Enemy/Attack/JumpAttack.h"
#include "../Object/Enemy/Attack/JumpAttackConstant.h"
#include "../Object/Enemy/Attack/FollowAttack.h"
#include "../Object/Enemy/Attack/FallDownAttack.h"
#include "../Object/Enemy/Attack/CrossAttack.h"
#include "../Object/Enemy/Attack/ThunderAroundAttack.h"
#include "../Object/Enemy/Attack/Wave.h"
#include "../Object/Enemy/Attack/FollowShot.h"
#include "../Object/Enemy/Attack/FallDownShot.h"
#include "../Object/Enemy/Attack/CrossLine.h"
#include "../Object/Enemy/Attack/ThunderAround.h"
#include "../Object/UI/EnemyHPUI.h"
#include"SceneGame.h"

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
	player_ = std::make_unique<PlayerBase>(0);
	player_->Init();
	//敵生成
	enemy_ = std::make_unique<EnemyBase>(player_->GetTransform());
	enemy_->Init();
	//敵HPUI生成
	enemyHPUI_ = std::make_unique<EnemyHPUI>(enemy_->GetMaxHP(), *enemy_);
	enemyHPUI_->Init();
	//カメラ設定
	auto& cam = SceneManager::GetInstance().GetCamera();
	cam.SetFollow(&player_->GetTransform(), &enemy_->GetTransform());
	cam.ChangeMode(Camera::MODE::FOLLOW);
	cam.SetPos(player_->GetTransform().pos);
	//スカイドーム
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
	ChangeCameraMode();
	//プレイヤー
	player_->Update();
	//敵
	enemy_->Update();

	CheckCollision();
	if (enemy_->GetHP() <= 0.0f)
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMECLEAR, true);
	}
	if (player_->GetHP() <= 0.0f)
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER, true);
	}
	
};

//描画処理
void SceneGame::Draw(void)
{


	skyDome_->Draw();
	player_->Draw();
	enemy_->Draw();
	DrawLine3D(player_->GetTransform().pos, enemy_->GetTransform().pos, GetColor(255, 0, 255));
	DebugDraw();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawCube3D(PlayerBase::MOVE_LIMIT_MIN, PlayerBase::MOVE_LIMIT_MAX, GetColor(255, 255, 255), GetColor(0, 0, 255), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	enemyHPUI_->Draw();
	DrawFormatString(0, 0, 0, "%f", player_->GetHP());
	DrawFormatString(0, 20, 0, "%d", GetDrawCallCount());
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

void SceneGame::ChangeCameraMode(void)
{
	auto& cam = SceneManager::GetInstance().GetCamera();
	auto& ins = KeyConfig::GetInstance();
	auto lockOnType = DataBank::GetInstance().GetLockOnType();
	auto controlType = DataBank::GetInstance().GetControlType();
	switch (lockOnType)
	{
	case DataBank::LOCK_ON_TYPE::FIXED:
		break;
	case DataBank::LOCK_ON_TYPE::PRESS:
		if (ins.IsNew(KeyConfig::CONTROL_TYPE::CHENGE_CAMERA_MODE, KeyConfig::JOYPAD_NO::PAD1, controlType))
		{
			cam.ChangeMode(Camera::MODE::TWO_TARGET_FOLLOW);
		}
		else if (ins.IsTrgUp(KeyConfig::CONTROL_TYPE::CHENGE_CAMERA_MODE, KeyConfig::JOYPAD_NO::PAD1, controlType))
		{
			cam.ChangeMode(Camera::MODE::FOLLOW);
		}
		break;
	case DataBank::LOCK_ON_TYPE::SWITCH:
		if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::CHENGE_CAMERA_MODE, KeyConfig::JOYPAD_NO::PAD1, controlType))
		{
			cam.ChangeMode(cam.GetMode() == Camera::MODE::FOLLOW ? Camera::MODE::TWO_TARGET_FOLLOW : Camera::MODE::FOLLOW);
		}
		break;
	default:
		break;
	}

}

void SceneGame::CheckCollision(void)
{
	//敵の攻撃の数
	auto enemyAttackNum = enemy_->GetAttackNum();
	//敵の攻撃格納
	std::vector<AttackBase*> enemyAttack;
	for (int i = 0;i < enemyAttackNum;i++)
	{
		enemyAttack.push_back(&enemy_->GetAttack(i));
	}
	//敵とプレイヤーの球の当たり判定
	int playerShotNum = player_->GetPlayerShotNum();
	for (int i = 0;i < playerShotNum;i++)
	{
		auto& playerShot = player_->GetPlayerShot(i);
		if (!playerShot.IsShot())continue;
		if (Utility::IsColSphere2Model(playerShot.GetTransform().pos, playerShot.GetRadius(), enemy_->GetTransform().modelId))
		{
			enemy_->Damage(2.0f);
			playerShot.Hit();
		}
	}
	if (!player_->IsDamageHit())
	{
		return;
	}
	//敵の攻撃とプレイヤーの当たり判定
	for (auto& attack : enemyAttack)
	{
		//攻撃しているか
		if (attack->GetState() == AttackBase::STATE::NONE || attack->GetState() == AttackBase::STATE::FINISH)
		{
			//していないから次へ
			continue;
		}
		//攻撃の形状で場合分け
		auto geo = attack->GetGeometory();
		switch (geo)
		{
		case AttackBase::GEOMETORY::SPHERE:
		{
			auto follow = dynamic_cast<FollowAttack*>(attack);
			if (follow != nullptr)
			{
				//追従との当たり判定
				int shotNum = follow->GetShotNum();
				for (int i = 0; i < shotNum; i++)
				{
					auto& transform = follow->GetShotTransform(i);
					if (Utility::IsColSphere2Sphere(player_->GetTransform().pos, PlayerBase::RADIUS, transform.pos, FollowAttack::RADIUS))
					{
						VECTOR vec = VNorm(VSub(player_->GetTransform().pos, transform.pos));
						vec.y = 0.5f;
						player_->Damage(FollowShot::DAMAGE, VNorm(vec));
						follow->HitShot(i);
					}
				}
				continue;
			}
			auto fall = dynamic_cast<FallDownAttack*>(attack);
			if (fall != nullptr)
			{
				//落下との当たり判定
				int shotNum = fall->GetFallDownShotNum();
				for (int i = 0; i < shotNum; i++)
				{
					if (fall->GetShotState(i) != FallDownShot::STATE::BLAST)
					{
						continue;
					}
					auto& transform = fall->GetShotTransform(i);
					if (Utility::IsColSphere2Sphere(player_->GetTransform().pos, PlayerBase::RADIUS, transform.pos, fall->GetShotRadius(i)))
					{
						VECTOR vec = VNorm(VSub(player_->GetTransform().pos, transform.pos));
						vec.y = 0.5f;
						player_->Damage(FallDownShot::DAMAGE, VNorm(vec));
					}
				}
				continue;
			}
			auto cross = dynamic_cast<CrossAttack*>(attack);
			if (cross != nullptr)
			{
				//十字との当たり判定
				int crossPointNum = cross->GetCrossLineNum();
				for (int i = 0; i < crossPointNum; i++)
				{
					auto& transform = cross->GetLineTransform(i);
					if (Utility::IsColSphere2Sphere(player_->GetTransform().pos, PlayerBase::RADIUS, transform.pos, CrossLine::RADIUS))
					{
						VECTOR vec = VNorm(VSub(player_->GetTransform().pos, transform.pos));
						vec.y = 0.5f;
						player_->Damage(CrossLine::DAMAGE, VNorm(vec));
					}
				}
				continue;
			}
		}
		break;
		case AttackBase::GEOMETORY::CIRCLE:
		{
			auto thunder = dynamic_cast<ThunderAroundAttack*>(attack);
			if (thunder != nullptr)
			{
				//サンダーとの当たり判定
				int thunderNum = thunder->GetThunderNum();
				for (int i = 0; i < thunderNum; i++)
				{
					auto& transform = thunder->GetThunderTransform(i);
					if (Utility::IsColSphere2Sphere(player_->GetTransform().pos, PlayerBase::RADIUS, transform.pos, ThunderAround::RADIUS))
					{
						VECTOR vec = VNorm(VSub(player_->GetTransform().pos, transform.pos));
						vec.y = 0.5f;
						player_->Damage(ThunderAround::DAMAGE, VNorm(vec));
					}
				}
			}
		}
			break;
		case AttackBase::GEOMETORY::CIRCUMFERENCE:
		{
			auto jump = dynamic_cast<JumpAttack*>(attack);
			if (jump != nullptr)
			{
				//Waveとの当たり判定
				if (player_->GetState() == PlayerBase::STATE::JUMP)
				{
					//ジャンプ中は当たらない
					continue;
				}
				int waveNum = jump->GetWaveNum();
				for (int i = 0; i < waveNum; i++)
				{
					float waveRadius;
					VECTOR wavePos;
					jump->GetWaveState(waveRadius, wavePos, i);
					VECTOR pPos = player_->GetTransform().pos;
					pPos.y = wavePos.y;
					if (Utility::IsColCircumference2Circle(wavePos, waveRadius, pPos, PlayerBase::RADIUS))
					{
						VECTOR vec = VNorm(VSub(player_->GetTransform().pos, wavePos));
						vec.y = 0.5f;
						player_->Damage(Wave::DAMAGE, VNorm(vec));
					}
				}
			}
			auto jumpC = dynamic_cast<JumpAttackConstant*>(attack);
			if (jumpC != nullptr)
			{
				//Waveとの当たり判定
				if (player_->GetState() == PlayerBase::STATE::JUMP)
				{
					//ジャンプ中は当たらない
					continue;
				}
				int waveNum = jumpC->GetWaveNum();
				for (int i = 0; i < waveNum; i++)
				{
					float waveRadius;
					VECTOR wavePos;
					jumpC->GetWaveState(waveRadius, wavePos, i);
					VECTOR pPos = player_->GetTransform().pos;
					pPos.y = wavePos.y;
					if (Utility::IsColCircumference2Circle(wavePos, waveRadius, pPos, PlayerBase::RADIUS))
					{
						VECTOR vec = VNorm(VSub(player_->GetTransform().pos, wavePos));
						vec.y = 0.5f;
						player_->Damage(Wave::DAMAGE, VNorm(vec));
					}
				}
			}
		}
		break;
		case AttackBase::GEOMETORY::MODEL:

			break;
		default:
			break;
		}
	}
	//敵とプレイヤーの当たり判定
	if (Utility::IsColSphere2Model(player_->GetTransform().pos, PlayerBase::RADIUS, enemy_->GetTransform().modelId))
	{
		VECTOR vec = VNorm(VSub(player_->GetTransform().pos, enemy_->GetTransform().pos));
		vec.y = 0.5f;
		player_->Damage(5.0f, VNorm(vec));
	}
}