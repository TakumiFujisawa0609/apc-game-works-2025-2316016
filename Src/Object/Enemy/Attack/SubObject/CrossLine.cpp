#include "../../../../Manager/ResourceManager.h"
#include "../../../../Manager/DrawTranslucentManager.h"
#include "../../../../Manager/SceneManager.h"
#include "../../../Common/Geometry/Sphere.h"
#include "../Type/CrossAttack.h"
#include "CrossLine.h"

CrossLine::CrossLine(VECTOR centerPos, float& rad, float initRad, int num, CrossAttack& parent) : centerPos_(centerPos), radian_(rad),parent_(parent)
{
	transform_ = std::make_shared<Transform>();
	//transform_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::FIRE_MODEL));
	//transform_->scl = VGet(SIZE_XZ, SIZE_Y, SIZE_XZ);
	transform_->Update();
	//material_ = std::make_unique<ModelMaterial>(
	//	"FireVS.cso", 2,
	//	"FirePS.cso", 1
	//);
	VECTOR pos = transform_->pos;
	//material_->AddConstBufVS({ time_, pos.x, pos.y, pos.z });
	//material_->AddConstBufVS({ RADIUS * 2.0f, 0.0f, 0.0f, 0.0f });
	//material_->AddConstBufPS({ time_, 0.0f, 1.0f, 1.0f });

	//material_->SetTextureBuf(3, ResourceManager::GetInstance().Load(ResourceManager::SRC::NOISE).handleId_);
	//renderer_ = std::make_shared<ModelRenderer>(
	//	transform_->modelId, *material_
	//);
	std::unique_ptr<Geometry>geo = std::make_unique<Sphere>(transform_->pos, RADIUS);
	MakeCollider(Collider::TAG::ENEMY_ATTACK, std::move(geo), { Collider::TAG::ENEMY,Collider::TAG::ENEMY_ATTACK });

	initRadian_ = initRad;
	num_ = num;
	damage_ = DAMAGE;
}

CrossLine::~CrossLine(void)
{
}

void CrossLine::Init(void)
{
}

void CrossLine::Update(void)
{
	transform_->pos.x = centerPos_.x + (num_ * RADIUS + DISTANCE) * cosf(radian_ + initRadian_ );
	transform_->pos.y = centerPos_.y;
	transform_->pos.z = centerPos_.z + (num_ * RADIUS + DISTANCE) * sinf(radian_ + initRadian_ );
	SetPolygonInfo();
	//VECTOR pos = transform_->pos;
	//material_->SetConstBufVS(0,{ time_, pos.x, pos.y, pos.z });
	//material_->SetConstBufPS(0, { time_, 0.0f,0.0f, 0.0f });
}

void CrossLine::Draw(void)
{
	//MV1DrawModel(transform_->modelId);
	//DrawTranslucentManager::GetInstance().Add(transform_, renderer_);
	parent_.AddVertexs(polInfo_);
}

void CrossLine::SetPolygonInfo(void)
{
	auto& vertexes = polInfo_.vertex;
	vertexes.clear();
	float radper = Utility::Deg2RadF(Utility::ONE_TRACK_DEG / VERTEX_NUM);
	for (int i = 0; i < VERTEX_NUM * 2; i++)
	{
		VERTEX3DSHADER vertex = {};
		VECTOR pos = transform_->pos;
		pos.x +=RADIUS * cosf(radper * static_cast<int>(i / static_cast<float>(2)));
		pos.y = i % 2 == 0 ? RADIUS : 0.0f;
		pos.z +=RADIUS * sinf(radper * static_cast<int>(i / static_cast<float>(2)));
		vertex.pos = pos;
		vertex.spos = { 0.0f,0.0f,0.0f,1.0f };
		vertex.norm = VNorm(VSub(vertex.pos, transform_->pos));
		vertex.tan = { 0.0f,0.0f,0.0f };
		vertex.binorm = vertex.norm;
		vertex.dif = COLOR_U8(0, 0, 0, 0);
		vertex.spc = COLOR_U8(0, 0, 0, 0);
		vertex.u = static_cast<float>((i / 2)% 2 == 0? 0.0f:1.0f);
		vertex.v = static_cast<float>(i % 2 == 0 ? 0.0f : 1.0f);
		vertex.su = vertex.u;
		vertex.sv = vertex.v;
		vertexes.push_back(vertex);
	}
	auto& indices = polInfo_.Indices;
	indices.clear();
	//indices.push_back(0);
	//indices.push_back(1);
	//indices.push_back(2);
	//indices.push_back(1);
	//indices.push_back(3);
	//indices.push_back(2);
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