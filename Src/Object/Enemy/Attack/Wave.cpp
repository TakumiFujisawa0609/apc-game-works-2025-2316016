#include <cmath>
#include "../../Manager/SceneManager.h"
#include "../../Utility/Utility.h"
#include "Wave.h"

Wave::Wave(VECTOR centerPos, SPEED_TYPE speedType, int color)
{
	centerPos_ = centerPos;
	speed_ = InitSpeed(speedType);
	color_ = color;
	time_ = 0.0f;
}

Wave::~Wave(void)
{
}

void Wave::Init(void)
{
}

void Wave::Update(void)
{
	//time_ += SceneManager::GetInstance().GetDeltaTime();
	time_ += 1.0f / 60;
}

void Wave::Draw(void)
{
	Utility::DrawCircle3DXZ(centerPos_, speed_ * time_, VERTEX_NUM, color_, false);
}

void Wave::Reset(void)
{
	time_ = 0.0f;
}

float Wave::InitSpeed(SPEED_TYPE speedType) const
{
	switch (speedType)
	{
	case SPEED_TYPE::SLOW:
		return SLOW_SPEED;
	case SPEED_TYPE::MIDIUM:
		return MIDIUM_SPEED;
	case SPEED_TYPE::FAST:
		return FAST_SPEED;
	default:
		return 0.0f;
	}
}
