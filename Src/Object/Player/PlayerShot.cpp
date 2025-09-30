#include "../Common/Gravity.h"
#include "../Common/Transform.h"
#include "PlayerShot.h"

PlayerShot::PlayerShot(VECTOR pPos, VECTOR tPos)
{
	transform_ = std::make_unique<Transform>();
	gravity_ = std::make_unique<Gravity>();
	gravity_->Init();
	gravity_->SetDir(VGet(0.0f, -1.0f, 0.0f));
	gravity_->SetInitPower(POWER);
	transform_->pos = pPos;
	startPos_ = pPos;
	targetPos_ = tPos;
	isDead_ = false;
	state_ = STATE::SHOT;
}

PlayerShot::~PlayerShot()
{
}

void PlayerShot::Init(void)
{
}

void PlayerShot::Update(void)
{
	if (isDead_ == true)
	{
		return;
	}
	gravity_->Update();
	VECTOR dir = VSub(targetPos_, startPos_);
	dir.y = 0.0f;
	dir = VNorm(dir);
	transform_->pos = VAdd(transform_->pos, VScale(dir, SPEED));
	transform_->pos = VAdd(transform_->pos, VScale(gravity_->GetDir(), gravity_->GetPower()));
	if (transform_->pos.y < 0.0f)
	{
		isDead_ = true;
	}
}

void PlayerShot::Draw(void)
{
	if (isDead_ == true)
	{
		return;
	}
	DrawSphere3D(transform_->pos, RADIUS, 16, GetColor(255, 255, 0), GetColor(255, 0, 0), true);
}

void PlayerShot::Hit(void)
{
	state_ = STATE::BLAST;
	isDead_ = true;	//爆発アニメーションまでの代用
}
