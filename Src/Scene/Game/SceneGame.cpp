#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../../Utility/Utility.h"
#include "../../Application.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SoundManager.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/DrawTranslucentManager.h"
#include "../../Manager/KeyConfig.h"
#include "../../Manager/Camera.h"
#include "../../Manager/DataBank.h"
#include "../../Renderer/PixelMaterial.h"
#include "../../Renderer/PixelRenderer.h"
#include "../../Object/Common/Gravity.h"
#include "../../Object/Player/PlayerBase.h"
#include "../../Object/SkyDome/SkyDome.h"
#include "../../Object/Stage/Stage.h"
#include "../../Object/Player/PlayerShot.h"
#include "../../Object/Enemy/EnemyBase.h"
#include "../../Object/Enemy/Attack/Type/AttackBase.h"
#include "../../Object/Enemy/Attack/Type/JumpAttack.h"
#include "../../Object/Enemy/Attack/Type/JumpAttackConstant.h"
#include "../../Object/Enemy/Attack/Type/FollowAttack.h"
#include "../../Object/Enemy/Attack/Type/FallDownAttack.h"
#include "../../Object/Enemy/Attack/Type/CrossAttack.h"
#include "../../Object/Enemy/Attack/Type/ThunderAroundAttack.h"
#include "../../Object/Enemy/Attack/Type/WaterSpritAttack.h"
#include "../../Object/Enemy/Attack/SubObject/Wave.h"
#include "../../Object/Enemy/Attack/SubObject/FollowShot.h"
#include "../../Object/Enemy/Attack/SubObject/FallDownShot.h"
#include "../../Object/Enemy/Attack/SubObject/CrossLine.h"
#include "../../Object/Enemy/Attack/SubObject/ThunderAround.h"
#include "../../Object/Enemy/Attack/SubObject/WaterSprit.h"
#include "../../Object/UI/EnemyHPUI.h"
#include "SceneGame.h"

SceneGame::SceneGame(void)
{
}

SceneGame::~SceneGame(void)
{
	DeleteGraph(postEffectScreen_);
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
	cam.ChangeMode(Camera::MODE::TWO_TARGET_FOLLOW);
	cam.SetPos(player_->GetTransform().pos);
	//スカイドーム
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Init();

	//ステージ
	stage_ = std::make_unique<Stage>();

	// ポストエフェクト用スクリーン
	postEffectScreen_ = MakeScreen(
		Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);
	// ポストエフェクト用(ビネット)
	vineMaterial_ = std::make_unique<PixelMaterial>("Vignette.cso", 1);
	vineMaterial_->AddConstBuf({ VIGNETTE_MAX_POW, player_->GetHP(), PlayerBase::MAX_HP, vignetteTime_});
	vineMaterial_->AddTextureBuf(SceneManager::GetInstance().GetMainScreen());
	vineRenderer_ = std::make_unique<PixelRenderer>(*vineMaterial_);
	vineRenderer_->MakeSquereVertex(
		Vector2(0, 0),
		Vector2(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y)
	);
	vignetteTime_ = 0.0f;
	return true;
}

//更新処理
void SceneGame::Update(void)
{
	//スカイドーム
	skyDome_->Update();
	//ステージ
	stage_->Update();
	ChangeCameraMode();
	//プレイヤー
	player_->Update();
	//敵
	enemy_->Update();

	KeyConfig& ins = KeyConfig::GetInstance();
	//全ての更新が終わったら当たり判定をする
	CheckCollision();

	vignetteTime_ += SceneManager::GetInstance().GetDeltaTime();

	ChangeScene();

	//メニューを開くキーが押されたらメニューを開く
	if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::OPEN_MENU, KeyConfig::JOYPAD_NO::PAD1))
	{
		SceneManager::GetInstance().PushScene(SceneManager::SCENE_ID::MENU);
	}
	//ヴィネットの定数バッファを更新する
	vineMaterial_->SetConstBuf(0,{ VIGNETTE_MAX_POW, player_->GetHP(), PlayerBase::MAX_HP, vignetteTime_ });
}



//描画処理
void SceneGame::Draw(void)
{
	int mainScreen = SceneManager::GetInstance().GetMainScreen();

	//各オブジェクトの描画
	skyDome_->Draw();
	stage_->Draw();
	player_->Draw();
	enemy_->Draw();

	DrawEffekseer3D();
	//DebugDraw();
	DrawTranslucentManager::GetInstance().Draw();
	//ポストエフェクト用のスクリーンに変える
	SetDrawScreen(postEffectScreen_);

	// 画面を初期化
	ClearDrawScreen();

	vineRenderer_->Draw();

	// メインに戻す
	SetDrawScreen(mainScreen);
	DrawGraph(0, 0, postEffectScreen_, false);

	//シェーダーの効果を受けないUI等を描画
	enemyHPUI_->Draw();
	//DrawFormatString(0, 0, 0, "%f", player_->GetHP());
	//DrawFormatString(0, 20, 0, "%d", GetDrawCallCount());
	//player_->UIDraw();
	enemy_->UIDraw();
}

void SceneGame::DebugDraw(void)
{
	//科リステージの描画
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
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawCube3D(PlayerBase::MOVE_LIMIT_MIN, PlayerBase::MOVE_LIMIT_MAX, GetColor(255, 255, 255), GetColor(0, 0, 255), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
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
		auto pShotPrePos = playerShot.GetPrePos();
		auto& pShotTransform = playerShot.GetTransform();
		auto result = MV1CollCheck_Line(enemy_->GetTransform().modelId, -1, pShotPrePos, pShotTransform.pos);
		if (result.HitFlag > 0)
		{
			VECTOR rot = Utility::VECTOR_ZERO;
			auto dir = VSub(pShotPrePos, pShotTransform.pos);
			rot.y = atan2f(dir.x, dir.z);
			enemy_->Damage(2.0f);
			playerShot.Hit(result.HitPosition,rot);
			SoundManager::GetInstance().Play(SoundManager::SRC::PSHOT_HIT, Sound::TIMES::ONCE);
		}
	}
	//プレイヤーの移動制限
	//VECTOR pPrePos = player_->GetPrePos();
	//pPrePos.y = 0.0f;
	VECTOR pPos = player_->GetTransform().pos;
	pPos.y = 0.0f;
	//auto result = MV1CollCheck_Line(waveId, -1, pPrePos, pPos);
	//if (result.HitFlag > 0)
	//{
	//	VECTOR hitPos = result.HitPosition;
	//	hitPos.y = player_->GetTransform().pos.y;
	//	player_->SetPos(VAdd(hitPos,VNorm(VSub(Utility::VECTOR_ZERO,hitPos))));
	//}
	auto dis = Utility::Distance(pPos, Utility::VECTOR_ZERO);
	if (dis > Stage::RADIUS)
	{
		VECTOR dir = VNorm(VSub(pPos,Utility::VECTOR_ZERO));
		VECTOR pos = VScale(dir, Stage::RADIUS);
		pos.y = player_->GetTransform().pos.y;
		player_->SetPos(pos);
	}

	if (!player_->IsDamageHit())
	{
		return;
	}
	for (auto& pPos : player_->GetCollisionSpherePositions())
	{
		//地面の位置
		VECTOR checkPos = pPos;
		checkPos.y = 0.0f;
		//敵の攻撃とプレイヤーの当たり判定
		for (auto& attack : enemyAttack)
		{
			//攻撃しているか
			if (attack->GetState() == AttackBase::STATE::NONE/* || attack->GetState() == AttackBase::STATE::FINISH*/)
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
					int shotNum = follow->GetSubObjectNum();
					for (int i = 0; i < shotNum; i++)
					{
						auto& transform = follow->GetShotTransform(i);
						if (Utility::IsColSphere2Sphere(pPos, PlayerBase::RADIUS, transform.pos, FollowAttack::RADIUS))
						{
							VECTOR vec = VNorm(VSub(pPos, transform.pos));
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
					int shotNum = fall->GetSubObjectNum();
					for (int i = 0; i < shotNum; i++)
					{
						if (fall->GetShotState(i) != FallDownShot::STATE::BLAST)
						{
							continue;
						}
						auto& transform = fall->GetShotTransform(i);
						if (Utility::IsColSphere2Sphere(pPos, PlayerBase::RADIUS, transform.pos, fall->GetShotRadius(i)))
						{
							VECTOR vec = VNorm(VSub(pPos, transform.pos));
							player_->Damage(FallDownShot::DAMAGE, VNorm(vec));
						}
					}
					continue;
				}
				auto cross = dynamic_cast<CrossAttack*>(attack);
				if (cross != nullptr)
				{
					//十字との当たり判定
					int crossPointNum = cross->GetSubObjectNum();
					for (int i = 0; i < crossPointNum; i++)
					{
						auto& transform = cross->GetLineTransform(i);
						if (Utility::IsColSphere2Sphere(pPos, PlayerBase::RADIUS, transform.pos, CrossLine::RADIUS))
						{
							VECTOR vec = VNorm(VSub(pPos, transform.pos));
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
					int thunderNum = thunder->GetSubObjectNum();
					for (int i = 0; i < thunderNum; i++)
					{
						auto& transform = thunder->GetThunderTransform(i);
						if (Utility::IsColSphere2Sphere(checkPos, PlayerBase::RADIUS, transform.pos, ThunderAround::RADIUS))
						{
							VECTOR vec = VNorm(VSub(pPos, transform.pos));
							player_->Damage(ThunderAround::DAMAGE, VNorm(vec));
						}
					}
				}
				auto water = dynamic_cast<WaterSpritAttack*>(attack);
				if (water != nullptr)
				{
					//水との当たり判定
					int waterNum = water->GetSubObjectNum();
					for (int i = 0; i < waterNum; i++)
					{
						auto& transform = water->GetWaterTransform(i);
						auto radius = water->GetWaterRadius(i);
						if (Utility::IsColSphere2Sphere(checkPos, PlayerBase::RADIUS, transform.pos,radius))
						{
							VECTOR vec = VNorm(VSub(pPos, transform.pos));
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
					if (player_->GetGravity().GetState() == Gravity::STATE::JUMP)
					{
						//ジャンプ中は当たらない
						continue;
					}
					int waveNum = jump->GetSubObjectNum();
					for (int i = 0; i < waveNum; i++)
					{
						float waveRadius;
						VECTOR wavePos;
						jump->GetWaveState(waveRadius, wavePos, i);
						if (Utility::IsColCircumference2Circle(wavePos, waveRadius, checkPos, PlayerBase::RADIUS))
						{
							VECTOR vec = VNorm(VSub(pPos, wavePos));
							player_->Damage(Wave::DAMAGE, VNorm(vec));
						}
					}
				}
				auto jumpC = dynamic_cast<JumpAttackConstant*>(attack);
				if (jumpC != nullptr)
				{
					//Waveとの当たり判定
					if (player_->GetGravity().GetState() == Gravity::STATE::JUMP)
					{
						//ジャンプ中は当たらない
						continue;
					}
					int waveNum = jumpC->GetSubObjectNum();
					for (int i = 0; i < waveNum; i++)
					{
						float waveRadius;
						VECTOR wavePos;
						jumpC->GetWaveState(waveRadius, wavePos, i);
						if (Utility::IsColCircumference2Circle(wavePos, waveRadius, checkPos, PlayerBase::RADIUS))
						{
							VECTOR vec = VNorm(VSub(pPos, wavePos));
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
		if (Utility::IsColSphere2Model(pPos, PlayerBase::RADIUS, enemy_->GetTransform().modelId))
		{
			VECTOR vec = VNorm(VSub(pPos, enemy_->GetTransform().pos));
			player_->Damage(5.0f, VNorm(vec));
		}
	}
}

void SceneGame::ChangeScene(void)
{
	//敵のHPが0を切ったらゲームクリアに移動
	if (enemy_->GetHP() <= 0.0f)
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMECLEAR, true);
	}
	//プレイヤーが死んだらゲームオーバーに移動
	if (player_->GetState() == PlayerBase::STATE::DEAD)
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER, true);
	}

}