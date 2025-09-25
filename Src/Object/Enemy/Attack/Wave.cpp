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

Wave::~Wave()
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
	float angleDeg = 360.0f / VERTEX_NUM;
	float angleRad = Utility::Deg2RadF(angleDeg);
	for (int i = 0; i < VERTEX_NUM; i++)
	{
		VECTOR vec1 = { cos(angleRad*(i - 1)),0.0f,sin(angleRad*(i - 1))};
		VECTOR vec2 = { cos(angleRad * i),0.0f,sin(angleRad * i) };
		VECTOR pos1 = VAdd(centerPos_, VScale(vec1, speed_ * time_));
		VECTOR pos2 = VAdd(centerPos_, VScale(vec2, speed_ * time_));
		DrawLine3D(pos1, pos2, color_);
	}
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
