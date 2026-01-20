#include <cmath>
#include "../../Manager/SceneManager.h"
#include "../../Renderer/ModelMaterial.h"
#include "../../Renderer/ModelRenderer.h"
#include "../../Utility/Utility.h"
#include "../../../Common/EffectController.h"
#include "../../../Common/Geometry/Circumference.h"
#include "Wave.h"

Wave::Wave(VECTOR centerPos, SPEED_TYPE speedType, AttackBase& parent) : SubObjectBase(parent)
{
	centerPos_ = centerPos;
	speed_ = InitSpeed(speedType);
	time_ = 0.0f;
	damage_ = DAMAGE;
	std::unique_ptr<Geometry> geo = std::make_unique<Circumference>(centerPos_, GetRadius());
	MakeCollider(Collider::TAG::ENEMY_ATTACK, std::move(geo), { Collider::TAG::ENEMY,Collider::TAG::ENEMY_ATTACK });
}

Wave::~Wave(void)
{
}

void Wave::Init(void)
{
}

void Wave::Update(void)
{
	time_ += SceneManager::GetInstance().GetDeltaTime();
	for(auto& col : colParam_)
	{
		auto circumference = static_cast<Circumference*>(col.geometry_.get());
		circumference->SetRadius(GetRadius());
	}
	SetPolygonInfo();
}

void Wave::Draw(void)
{
	//Utility::DrawCircle3DXZ(centerPos_, speed_ * time_, VERTEX_NUM, color_, false);
	parent_.AddVertexs(polInfo_);
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
	case SPEED_TYPE::RANDOM:
		return GetRand(static_cast<int>(FAST_SPEED - SLOW_SPEED)) + SLOW_SPEED;
	default:
		return 0.0f;
	}
}

void Wave::SetPolygonInfo(void)
{
	auto& vertexes = polInfo_.vertex;
	vertexes.clear();
	float radper = Utility::Deg2RadF(Utility::ONE_TRACK_DEG / VERTEX_NUM);
	for (int i = 0; i < VERTEX_NUM * HEIGHT_VERTEX_NUM; i++)
	{
		VERTEX3DSHADER vertex = {};
		VECTOR pos = centerPos_;
		pos.x += GetRadius() * cosf(radper * static_cast<int>(i / static_cast<float>(HEIGHT_VERTEX_NUM)));
		pos.y = i % HEIGHT_VERTEX_NUM == 0 ? HEIGHT : 0.0f;
		pos.z += GetRadius() * sinf(radper * static_cast<int>(i / static_cast<float>(HEIGHT_VERTEX_NUM)));
		vertex.pos = pos;
		vertex.spos = { 0.0f,0.0f,0.0f,1.0f };
		vertex.norm = VNorm(VSub(vertex.pos, centerPos_));
		vertex.tan = { 0.0f,0.0f,0.0f };
		vertex.binorm = vertex.norm;
		vertex.dif = COLOR_U8(0, 0, 0, 0);
		vertex.spc = COLOR_U8(0, 0, 0, 0);
		vertex.u = static_cast<float>((i / 2) % 2 == 0 ? 0.0f : 1.0f);
		vertex.v = static_cast<float>(i % 2 == 0 ? 0.0f : 1.0f);
		vertex.su = vertex.u;
		vertex.sv = vertex.v;
		vertexes.push_back(vertex);
	}
	auto& indices = polInfo_.Indices;
	indices.clear();
	for (int i = 0; i < VERTEX_NUM; i++)
	{
		int curTop = i * 2;
		int curBot = curTop + 1;

		int nextTop = ((i + 1) % VERTEX_NUM) * 2;
		int nextBot = nextTop + 1;

		// ŽOŠpŒ`1
		indices.push_back(curTop);
		indices.push_back(nextTop);
		indices.push_back(curBot);

		// ŽOŠpŒ`2
		indices.push_back(nextTop);
		indices.push_back(nextBot);
		indices.push_back(curBot);
	}
}
