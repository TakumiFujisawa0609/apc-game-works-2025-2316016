#include "../../Application.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Renderer/PixelMaterial.h"
#include "../../Renderer/PixelRenderer.h"
#include "../Enemy/EnemyBase.h"
#include "EnemyHPUI.h"

EnemyHPUI::EnemyHPUI(float maxHP, EnemyBase& enemy) :enemy_(enemy)
{
	maxHP_ = maxHP;
	lightDir_ = INIT_LIGHT_DIR;
	pos_ = { MARGIN_X,MARGIN_Y };
	size_ = { Application::SCREEN_SIZE_X - (MARGIN_X * 2),SIZE_Y };
	screenHandle_ = MakeScreen(size_.x, size_.y,true);
	ResourceManager& ins = ResourceManager::GetInstance();
	uiHandle_ = ins.Load(ResourceManager::SRC::ENEMY_HP_UI_IMG).handleId_;
	uiMaskHandle_ = ins.Load(ResourceManager::SRC::ENEMY_HP_UI_MASK_IMG).handleId_;
	uiNormalHandle_ = ins.Load(ResourceManager::SRC::ENEMY_HP_UI_NORMAL_IMG).handleId_;
	InitMaskImageMinMax();
	rate_ = enemy_.GetHP() / maxHP_;
	rate_ = (rate_ < 0.0f) ? 0.0f : rate_;
	// ポストエフェクト用(ビネット)
	uiMaterial_ = std::make_unique<PixelMaterial>("EnemyHPUI.cso", 3);
	uiMaterial_->AddConstBuf({ rate_, GAGE_COL.x, GAGE_COL.y, GAGE_COL.z });
	uiMaterial_->AddConstBuf({ gageMin_,gageMax_,0.0f,0.0f});
	uiMaterial_->AddConstBuf({ lightDir_.x,lightDir_.y,lightDir_.z,0.0f});
	uiMaterial_->AddTextureBuf(screenHandle_);
	uiMaterial_->AddTextureBuf(uiHandle_);
	uiMaterial_->AddTextureBuf(uiMaskHandle_);
	uiMaterial_->AddTextureBuf(uiNormalHandle_);
	uiRenderer_ = std::make_unique<PixelRenderer>(*uiMaterial_);
	uiRenderer_->MakeSquereVertex(
		Vector2(0, 0),
		size_
	);
}

EnemyHPUI::~EnemyHPUI()
{
	DeleteGraph(screenHandle_);
}

void EnemyHPUI::Init(void)
{
}

void EnemyHPUI::Update(void)
{
	//体力の割合を取得
	rate_ = enemy_.GetHP() / maxHP_;
	rate_ = (rate_ < 0.0f) ? 0.0f : rate_;
	auto& inc = SceneManager::GetInstance();
	float deltaTime = inc.GetDeltaTime();
	float rad = atan2(lightDir_.x, lightDir_.z);
	rad += Utility::Deg2RadF(ROT_DEG_SEC * deltaTime);
	lightDir_.x = sinf(rad);
	lightDir_.z = cosf(rad);
	uiMaterial_->SetConstBuf(0,{ rate_, GAGE_COL.x, GAGE_COL.y, GAGE_COL.z });
	uiMaterial_->SetConstBuf(2,{ lightDir_.x,lightDir_.y,lightDir_.z,0.0f });
}

void EnemyHPUI::Draw(void)
{
	//描画
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	//DrawBox(pos_.x, pos_.y, pos_.x + size_.x, pos_.y + size_.y, GetColor(0, 0, 0), true);
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//DrawBox(pos_.x + 2.0f, pos_.y + 2.0f, pos_.x + (size_.x - 2.0f) * rate, pos_.y + size_.y - 2.0f, GetColor(255, 0, 0), true);
	int mainScreen = SceneManager::GetInstance().GetMainScreen();
	SetDrawScreen(screenHandle_);
	ClearDrawScreen();
	uiRenderer_->Draw();
	SetDrawScreen(mainScreen);
	DrawGraph(MARGIN_X, MARGIN_Y, screenHandle_, true);
	//DrawExtendGraph(MARGIN_X, MARGIN_Y, MARGIN_X + size_.x, MARGIN_Y + size_.y, uiHandle_, true);
}

void EnemyHPUI::InitMaskImageMinMax(void)
{
	int maskImg = LoadSoftImage(ResourceManager::GetInstance().Load(ResourceManager::SRC::ENEMY_HP_UI_MASK_IMG).path_.c_str());
	int width, high;
	GetSoftImageSize(maskImg, &width, &high);
	int min = width;
	int max = 0;
	for (int x = 0; x < width;x++)
	{
		for (int y = 0; y < high;y++)
		{
			int r, g, b, a;
			GetPixelSoftImage(maskImg, x, y, &r, &g, &b, &a);
			if (r <static_cast<int>(255 * 0.9f))
			{
				continue;
			}
			min = std::min(min, x);
			max = std::max(max, x);
			break;
		}
	}
	gageMin_ = static_cast<float>(min) / width;
	gageMax_ = static_cast<float>(max) / width;
	DeleteSoftImage(maskImg);
}
