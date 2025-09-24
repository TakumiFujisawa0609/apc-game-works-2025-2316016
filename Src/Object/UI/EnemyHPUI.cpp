#include "../../Application.h"
#include "../Enemy/EnemyBase.h"
#include "EnemyHPUI.h"

EnemyHPUI::EnemyHPUI(float maxHP, EnemyBase& enemy) :enemy_(enemy)
{
	maxHP_ = maxHP;
	pos_ = { MARGIN_X,MARGIN_Y };
	size_ = { Application::SCREEN_SIZE_X - (MARGIN_X * 2),SIZE_Y };
}

EnemyHPUI::~EnemyHPUI()
{
}

void EnemyHPUI::Init(void)
{
}

void EnemyHPUI::Update(void)
{
}

void EnemyHPUI::Draw(void)
{
	float rate = enemy_.GetHP() / maxHP_;
	rate = (rate < 0.0f) ? 0.0f : rate;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(pos_.x, pos_.y, pos_.x + size_.x, pos_.y + size_.y, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawBox(pos_.x + 2.0f, pos_.y + 2.0f, pos_.x + (size_.x - 2.0f) * rate, pos_.y + size_.y - 2.0f, GetColor(255, 0, 0), true);
}
