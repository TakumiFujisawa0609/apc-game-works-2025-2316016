#include <memory>
#include "../../Utility/Utility.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/DrawTranslucentManager.h"
#include "../../Renderer/ModelMaterial.h"
#include "../../Renderer/ModelRenderer.h"
#include "../../../Common/EffectController.h"
#include "../../../Common/Transform.h"
#include "../../../Common/Geometry/Cylinder.h"
#include "../../../Stage/Stage.h"
#include "../Type/WaterSpritAttack.h"
#include "WaterSprit.h"

WaterSprit::WaterSprit(VECTOR direction, VECTOR startPos , float speed, WaterSpritAttack&parent): parent_(parent)
{
	size_ = INIT_SIZE;
	transform_ = std::make_shared<Transform>();
	//transform_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::WATER_SPRIT_THUNDER_MODEL));
	transform_->pos = startPos;
	//float scl = size_ / MODEL_RADIUS;
	//transform_->scl = VGet(scl, 1.0f, scl);
	//transform_->Update();
	//material_ = std::make_unique<ModelMaterial>(
	//	"WaterSplitVS.cso", 2,
	//	"WaterSplitPS.cso", 2
	//);
	//material_->AddConstBufVS({ 1.0f, 10.0f,0.0f,0.0f });
	//material_->AddConstBufVS({ startPos_.x, startPos_.y, startPos_.z,0.0f });
	//material_->AddConstBufPS({ time_,TIME_SCALE, 0.0f, 0.0f });
	//material_->SetTextureBuf(3, ResourceManager::GetInstance().Load(ResourceManager::SRC::NOISE).handleId_);
	//renderer_ = std::make_shared<ModelRenderer>(
	//	transform_->modelId, *material_
	//);
	speed_ = speed;
	startPos_ = startPos;
	dir_ = VNorm(direction);
	time_ = ALIVE_TIME;
	isEnd_ = false;
	damage_ = DAMAGE;
	std::unique_ptr<Geometry>geo = std::make_unique<Cylinder>(transform_->pos, size_);
	MakeCollider(Collider::TAG::ENEMY_ATTACK, std::move(geo), { Collider::TAG::ENEMY,Collider::TAG::ENEMY_ATTACK });
	SetPolygonInfo();
}

WaterSprit::~WaterSprit(void)
{
}

void WaterSprit::Init(void)
{
}

void WaterSprit::Update(void)
{
	auto deltaTime = SceneManager::GetInstance().GetDeltaTime();
	size_ += SECOND_PER_SIZE * deltaTime;
	time_ -= deltaTime;
	transform_->pos = VAdd(transform_->pos, VScale(dir_, speed_));
	//transform_->quaRot = Quaternion(VGet(0.0f, rot_, 0.0f));
	//float scl = size_ / MODEL_RADIUS;
	//transform_->scl = VGet(scl, 1.0f, scl);
	//transform_->Update();
	if (time_ < 0.0f || Utility::Distance(startPos_, transform_->pos) > Stage::RADIUS)
	{
		isEnd_ = true;
	}
	//auto pos = transform_->pos;
	//material_->SetConstBufVS(1, { pos.x, pos.y, pos.z ,0.0f });
	//material_->SetConstBufPS(0, { time_, TIME_SCALE,0.0f, 0.0f });
	for (auto& col : colParam_)
	{
		auto& geo = dynamic_cast<Cylinder&>(*col.geometry_);
		geo.SetRadius(size_);
	}
	SetPolygonInfo();
}

void WaterSprit::Draw(void)
{
	Utility::DrawCircle3DXZ(transform_->pos, size_, 16, GetColor(0, 0, 255), true);
	//DrawTranslucentManager::GetInstance().Add(transform_,renderer_);
	//renderer_->Draw();
	parent_.AddVertexs(polInfo_);
}


void WaterSprit::SetPolygonInfo(void)
{

	auto& vertexes = polInfo_.vertex;
	vertexes.clear();
	float radper = Utility::Deg2RadF(Utility::ONE_TRACK_DEG / VERTEX_NUM);
	for (int i = 0; i < (VERTEX_NUM + 1) * 2; i++)
	{
		VERTEX3DSHADER vertex = {};
		VECTOR pos = transform_->pos;
		pos.x += size_ * cosf(radper * static_cast<int>(i / static_cast<float>(2))) * (i % 2 == 0 ? TIMES_TOP_SIZE : 1.0f);
		pos.y = i % 2 == 0 ? HEIGHT : 0.0f;
		pos.z += size_ * sinf(radper * static_cast<int>(i / static_cast<float>(2))) * (i % 2 == 0 ? TIMES_TOP_SIZE : 1.0f);
		vertex.pos = pos;
		vertex.spos = { 0.0f,0.0f,0.0f,1.0f };
		vertex.norm = VNorm(VSub(vertex.pos, transform_->pos));
		vertex.tan = { 0.0f,0.0f,0.0f };
		vertex.binorm = vertex.norm;
		vertex.dif = COLOR_U8(0, 0, 0, 0);
		vertex.spc = COLOR_U8(0, 0, 0, 0);
		//vertex.u = static_cast<float>((i / 2) % 2 == 0 ? 0.0f : 1.0f);
		//vertex.v = static_cast<float>(i % 2 == 0 ? 0.0f : 1.0f);
		vertex.u = static_cast<float>(1.0f /(static_cast<float>(VERTEX_NUM) /(i / 2)));
		vertex.v = static_cast<float>(i % 2 == 0 ? 0.0f : 1.0f);
		vertex.su = vertex.u;
		vertex.sv = vertex.v;
		vertexes.push_back(vertex);
	}
	auto& indices = polInfo_.Indices;
	indices.clear();
	for (int i = 0; i < (VERTEX_NUM); i++)
	{
		int curTop = i * 2;
		int curBot = curTop + 1;

		int nextTop = ((i + 1)/* % VERTEX_NUM*/) * 2;
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