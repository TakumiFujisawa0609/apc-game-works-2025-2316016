#include<DxLib.h>
#include "../Utility/Utility.h"
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SoundManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/KeyConfig.h"
#include "../Manager/Camera.h"
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
#include "../Object/Enemy/Attack/Wave.h"
#include "../Object/Enemy/Attack/FollowShot.h"
#include "../Object/Enemy/Attack/FallDownShot.h"
#include "../Object/Enemy/Attack/CrossLine.h"
#include "../Object/UI/EnemyHPUI.h"
#include"SceneGame.h"

SceneGame::SceneGame(void)
{
}

SceneGame::~SceneGame(void)
{

}


//èâä˙âªèàóù(èââÒÇÃ1ìxÇÃÇ›é¿çsÇ≥ÇÍÇÈ)
bool SceneGame::Init(void)
{
	SceneBase::Init();
	//ÉvÉåÉCÉÑÅ[ê∂ê¨
	player_ = std::make_unique<PlayerBase>(0);
	player_->Init();
	//ìGê∂ê¨
	enemy_ = std::make_unique<EnemyBase>(player_->GetTransform());
	enemy_->Init();
	//ìGHPUIê∂ê¨
	enemyHPUI_ = std::make_unique<EnemyHPUI>(enemy_->GetMaxHP(), *enemy_);
	enemyHPUI_->Init();
	//ÉJÉÅÉâê›íË
	auto& cam = SceneManager::GetInstance().GetCamera();
	cam.ChangeMode(Camera::MODE::FOLLOW);
	cam.SetFollow(&player_->GetTransform(), &enemy_->GetTransform());
	cam.SetPos(player_->GetTransform().pos);
	//ÉXÉJÉCÉhÅ[ÉÄ
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Init();
	return true;
}

//çXêVèàóù
void SceneGame::Update(void)
{
	skyDome_->Update();
	//InputManager& ins = InputManager::GetInstance();
	KeyConfig& ins = KeyConfig::GetInstance();
	ChangeCameraMode();
	//ÉvÉåÉCÉÑÅ[
	player_->Update();
	//ìG
	enemy_->Update();

	CheckCollision();
	if (enemy_->GetHP() <= 0.0f)
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE, true);
	}
	if (player_->GetHP() <= 0.0f)
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE, true);
	}
	
};

//ï`âÊèàóù
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
	if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::CHENGE_CAMERA_MODE, KeyConfig::JOYPAD_NO::PAD1))
	{
		cam.ChangeMode(Camera::MODE::TWO_TARGET_FOLLOW);
	}
	else if (ins.IsTrgUp(KeyConfig::CONTROL_TYPE::CHENGE_CAMERA_MODE, KeyConfig::JOYPAD_NO::PAD1))
	{
		cam.ChangeMode(Camera::MODE::FOLLOW);
	}
}

void SceneGame::CheckCollision(void)
{
	//ìGÇÃçUåÇÇÃêî
	auto enemyAttackNum = enemy_->GetAttackNum();
	//ìGÇÃçUåÇäiî[
	std::vector<AttackBase*> enemyAttack;
	for (int i = 0;i < enemyAttackNum;i++)
	{
		enemyAttack.push_back(&enemy_->GetAttack(i));
	}
	//ìGÇ∆ÉvÉåÉCÉÑÅ[ÇÃãÖÇÃìñÇΩÇËîªíË
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
	//ìGÇÃçUåÇÇ∆ÉvÉåÉCÉÑÅ[ÇÃìñÇΩÇËîªíË
	for (auto& attack : enemyAttack)
	{
		//çUåÇÇµÇƒÇ¢ÇÈÇ©
		if (attack->GetState() == AttackBase::STATE::NONE || attack->GetState() == AttackBase::STATE::FINISH)
		{
			//ÇµÇƒÇ¢Ç»Ç¢Ç©ÇÁéüÇ÷
			continue;
		}
		//çUåÇÇÃå`èÛÇ≈èÍçáï™ÇØ
		auto geo = attack->GetGeometory();
		switch (geo)
		{
		case AttackBase::GEOMETORY::SPHERE:
		{
			auto follow = dynamic_cast<FollowAttack*>(attack);
			if (follow != nullptr)
			{
				//í«è]Ç∆ÇÃìñÇΩÇËîªíË
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
				//è\éöÇ∆ÇÃìñÇΩÇËîªíË
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
				//óéâ∫Ç∆ÇÃìñÇΩÇËîªíË
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
			break;
		case AttackBase::GEOMETORY::CIRCUMFERENCE:
		{
			auto jump = dynamic_cast<JumpAttack*>(attack);
			if (jump != nullptr)
			{
				//WaveÇ∆ÇÃìñÇΩÇËîªíË
				if (player_->GetState() == PlayerBase::STATE::JUMP)
				{
					//ÉWÉÉÉìÉvíÜÇÕìñÇΩÇÁÇ»Ç¢
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
				//WaveÇ∆ÇÃìñÇΩÇËîªíË
				if (player_->GetState() == PlayerBase::STATE::JUMP)
				{
					//ÉWÉÉÉìÉvíÜÇÕìñÇΩÇÁÇ»Ç¢
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
	//ìGÇ∆ÉvÉåÉCÉÑÅ[ÇÃìñÇΩÇËîªíË
	if (Utility::IsColSphere2Model(player_->GetTransform().pos, PlayerBase::RADIUS, enemy_->GetTransform().modelId))
	{
		VECTOR vec = VNorm(VSub(player_->GetTransform().pos, enemy_->GetTransform().pos));
		vec.y = 0.5f;
		player_->Damage(5.0f, VNorm(vec));
	}
}