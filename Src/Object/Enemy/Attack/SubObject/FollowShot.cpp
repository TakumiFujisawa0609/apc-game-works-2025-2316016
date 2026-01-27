#include <memory>
#include "../../../../Manager/ResourceManager.h"
#include "../../../../Manager/SceneManager.h"
#include "../../Renderer/ModelMaterial.h"
#include "../../Renderer/ModelRenderer.h"
#include "../../../Common/Transform.h"
#include "../../../Common/EffectController.h"
#include "../../../Common/Geometry/Sphere.h"
#include "../Type/FollowAttack.h"
#include "FollowShot.h"

FollowShot::FollowShot(std::weak_ptr<Transform> target, SPEED_TYPE speed, VECTOR startPos, FollowAttack& parent) : target_(target), SubObjectBase(parent)
{
	transform_ = std::make_shared<Transform>();
	transform_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::CHICKIN));
	transform_->pos = startPos;
	transform_->pos.y += RADIUS;
	time_ = ATTACK_TIME;
	transform_->Update();
	std::unique_ptr<Geometry>geo = std::make_unique<Sphere>(transform_->pos, RADIUS);
	MakeCollider(Collider::TAG::ENEMY_ATTACK, std::move(geo), { Collider::TAG::ENEMY,Collider::TAG::ENEMY_ATTACK });
	speed_ = InitSpeed(speed);
	damage_ = DAMAGE;
	state_ = STATE::SHOT;
}

FollowShot::~FollowShot(void)
{
}

void FollowShot::Init(void)
{
}

void FollowShot::Update(void)
{
	time_ -= SceneManager::GetInstance().GetDeltaTime();
	auto dir = VSub(target_.lock()->pos, transform_->pos);
	dir.y = 0.0f;
	transform_->pos = VAdd(transform_->pos, VScale(VNorm(dir), speed_));
	transform_->Update();
	if (time_ < 0.0f)
	{
		state_ = STATE::DEAD;
	}
	SetPolygonInfo();
}

void FollowShot::Draw(void)
{
	//MV1DrawModel(transform_->modelId);
	parent_.AddVertexs(polInfo_);
}

void FollowShot::Hit(void)
{
	state_ = STATE::BLAST;
}

float FollowShot::InitSpeed(SPEED_TYPE speedType) const
{
	switch (speedType)
	{
	case SPEED_TYPE::SLOW:
		return SLOW_SPEED;
	case SPEED_TYPE::MIDIUM:
		return MIDIUM_SPEED;
	case SPEED_TYPE::FAST:
		return FAST_SPEED;
	case SPEED_TYPE::RANDOM:
		return GetRand(static_cast<int>((FAST_SPEED - SLOW_SPEED)*10))/10 + SLOW_SPEED;
	default:
		return 0.0f;
	}
}

void FollowShot::SetPolygonInfo(void)
{
	auto& vertices = polInfo_.vertex;
	vertices.clear();
	auto& indices = polInfo_.Indices;
	indices.clear();
	VECTOR center = transform_->pos;
	const float PI = DX_PI_F;

	// =========================
	// 頂点生成
	// =========================
	for (int lat = 0; lat <= VERTEX_NUM; lat++)
	{
		float v = static_cast<float>(lat) / VERTEX_NUM;
		float theta = PI * v;

		float sinT = sinf(theta);
		float cosT = cosf(theta);

		for (int lon = 0; lon <= VERTEX_NUM; lon++)
		{
			float u = static_cast<float>(lon) / VERTEX_NUM;
			float phi = 2.0f * PI * u;

			float sinP = sinf(phi);
			float cosP = cosf(phi);

			VECTOR pos;
			pos.x = center.x + RADIUS * sinT * cosP;
			pos.y = center.y + RADIUS * cosT;
			pos.z = center.z + RADIUS * sinT * sinP;

			VERTEX3DSHADER vtx{};
			vtx.pos = pos;
			vtx.norm = VNorm(VSub(pos, center));
			vtx.tan = { 0,0,0 };
			vtx.binorm = vtx.norm;
			vtx.dif = COLOR_U8(255, 255, 255, 255);
			vtx.spc = COLOR_U8(0, 0, 0, 0);
			vtx.u = u;
			vtx.v = v;
			vtx.su = u;
			vtx.sv = v;
			vtx.spos = { 0,0,0,1 };

			vertices.push_back(vtx);
		}
	}

	// =========================
	// インデックス生成
	// =========================
	int stride = VERTEX_NUM + 1;

	for (int lat = 0; lat < VERTEX_NUM; lat++)
	{
		for (int lon = 0; lon < VERTEX_NUM; lon++)
		{
			int i0 = lat * stride + lon;
			int i1 = i0 + 1;
			int i2 = i0 + stride;
			int i3 = i2 + 1;

			// 三角形①
			indices.push_back(i0);
			indices.push_back(i2);
			indices.push_back(i1);

			// 三角形②
			indices.push_back(i1);
			indices.push_back(i2);
			indices.push_back(i3);
		}
	}
}
