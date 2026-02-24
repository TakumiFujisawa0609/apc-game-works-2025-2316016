#include "../Common/Gravity.h"
#include "../Common/Geometry/Line3D.h"
#include "PlayerShot.h"
#include "ShotGuide.h"

ShotGuide::ShotGuide(VECTOR& handPos) : handPos_(handPos)
{
	isHit_ = false;
	targetPos_ = VGet(0.0f, 0.0f, 0.0f);

	transform_ = std::make_shared<Transform>();
	transform_->pos = handPos;
	gravity_ = std::make_unique<Gravity>();
	gravity_->SetDir(VGet(0.0f, -1.0f, 0.0f));
	gravity_->ChengeState(Gravity::STATE::NONE);
}

ShotGuide::~ShotGuide()
{
}

void ShotGuide::Init(void)
{
}

void ShotGuide::Update(void)
{
	transform_->pos = handPos_;
	isHit_ = false;
	guidePoints_.clear();
	gravity_->ChengeState(Gravity::STATE::NONE);
	for(auto& colParam : colParam_)
	{
		colParam.collider_->Kill();
	}
	VECTOR dir = VSub(targetPos_, transform_->pos);
	gravity_->SetInitPower(PlayerShot::POWER + (dir.y < 0.0f ? 0.0f : dir.y));
	gravity_->Init();
	dir.y = 0.0f;
	dir = VNorm(dir);
	guidePoints_.push_back(transform_->pos);
	int guideCount = 0;
	for(;;)
	{
		gravity_->Update();
		//•ûŒü‚ðŽæ“¾
		//À•W‚ÌXV
		transform_->pos = VAdd(transform_->pos, VScale(dir, PlayerShot::SPEED));
		VECTOR gravityDir = gravity_->GetDir();
		float gravityPow = gravity_->GetPower();
		transform_->pos = VAdd(transform_->pos, VScale(gravityDir, gravityPow));
		guidePoints_.push_back(transform_->pos);
		if (transform_->pos.y < 0.0f)
		{
			break;
		}
		guideCount++;
		if (guideCount > MAX_GUIDE_NUM)
		{
			break;
		}
	}
	int guideNum = static_cast<int>(guidePoints_.size());
	for (int i = 0; i < guideNum - 1; i++)
	{
		std::unique_ptr<Geometry> geo = std::make_unique<Line3D>(guidePoints_[i], guidePoints_[i + 1]);
		MakeCollider(Collider::TAG::PLAYER_ATTACK_GUIDE, std::move(geo), { Collider::TAG::PLAYER,Collider::TAG::PLAYER_ATTACK,Collider::TAG::ENEMY_ATTACK ,Collider::TAG::GATE,Collider::TAG::PLAYER_ATTACK_GUIDE,Collider::TAG::PLAYER_LAND });
	}
}

void ShotGuide::Draw(void)
{
	int guideNum = static_cast<int>(guidePoints_.size());
	int color = isHit_ ? HIT_COLOR : NON_HIT_COLOR;
	SetUseLighting(false);
	for (int i = 0; i < guideNum - 1; i++)
	{
		//DrawLine3D(guidePoints_[i], guidePoints_[i + 1], color);
		//DrawPixel3D(guidePoints_[i], color);
		DrawSphere3D(guidePoints_[i], PlayerShot::RADIUS, 8, color, color, true);
	}
	SetUseLighting(true);
}

void ShotGuide::OnHit(const std::weak_ptr<Collider> _hitCol, VECTOR hitPos)
{
	isHit_ = true;
}
