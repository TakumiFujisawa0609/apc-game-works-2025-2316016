#include "../../Utility/Utility.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/DrawTranslucentManager.h"
#include "../../Renderer/ModelMaterial.h"
#include "../../Renderer/ModelRenderer.h"
#include "../../../Common/EffectController.h"
#include "../../../Common/Geometry/Cylinder.h"
#include "../Type/ThunderAroundAttack.h"
#include "ThunderAround.h"

ThunderAround::ThunderAround(VECTOR targetPos, VECTOR initPos,  float initRad, ThunderAroundAttack& parent) : initRad_(initRad),parent_(parent)
{
	sumTime_ = 0.0f;
	targetPos_ = targetPos;
	targetPos_.y = 0.0f;
	transform_ = std::make_shared<Transform>();
	//transform_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::WATER_SPRIT_THUNDER_MODEL));
	transform_->pos = initPos;
	transform_->pos.y = 0.0f;
	//transform_->scl = {MODEL_SCALE_XZ,1.0f,MODEL_SCALE_XZ};
	transform_->Update();

	//material_ = std::make_unique<ModelMaterial>(
	//	"ThunderVS.cso", 2,
	//	"ThunderPS.cso", 1
	//);
	//VECTOR pos = transform_->pos;
	//material_->AddConstBufVS({ TEXTURE_SCALE_X,TEXTURE_SCALE_Y,RADIUS,0.0f });
	//material_->AddConstBufVS({ pos.x,pos.y,pos.z,static_cast<float>(rand())});
	//material_->AddConstBufPS({ sumTime_,TIME_SCALE, THRESHOLD, 0.0f });
	////material_->SetTextureBuf(3, ResourceManager::GetInstance().Load(ResourceManager::SRC::NOISE).handleId_);
	//renderer_ = std::make_shared<ModelRenderer>(
	//	transform_->modelId, *material_
	//);

	std::unique_ptr<Geometry>geo = std::make_unique<Cylinder>(transform_->pos, RADIUS);
	MakeCollider(Collider::TAG::ENEMY_ATTACK, std::move(geo), { Collider::TAG::ENEMY,Collider::TAG::ENEMY_ATTACK });
	damage_ = DAMAGE;
	rad_ = 0.0f;
	dir_ = VNorm(VSub(targetPos, initPos));
	distance_ = static_cast<float>(Utility::Distance(targetPos,initPos));
	distance_ += MOVE_ADD_DISTANCE;
	changeState_[STATE::REDUCTION] = std::bind(&ThunderAround::ChangeStateReduction, this);
	changeState_[STATE::ROTATION] = std::bind(&ThunderAround::ChangeStateRotation, this);
	changeState_[STATE::EXPANSION] = std::bind(&ThunderAround::ChangeStateExpansion, this);
	changeState_[STATE::DEAD] = std::bind(&ThunderAround::ChangeStateDead, this);
	ChangeState(STATE::REDUCTION);
	SetPolygonInfo();
}

ThunderAround::~ThunderAround(void)
{
}

void ThunderAround::Init(void)
{
}

void ThunderAround::Update(void)
{
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();
	time_ -= deltaTime;
	sumTime_ += deltaTime;
	updateState_();
	transform_->Update();
	VECTOR pos = transform_->pos;
	//material_->SetConstBufVS(1,{ pos.x,pos.y,pos.z,static_cast<float>(rand()) });
	//material_->SetConstBufPS(0,{ sumTime_,TIME_SCALE, THRESHOLD, 0.0f });
	SetPolygonInfo();
}

void ThunderAround::Draw(void)
{
	//Utility::DrawCircle3DXZ(transform_->pos, RADIUS, VERTEX_NUM, GetColor(255, 0, 0), true);
	//DrawTranslucentManager::GetInstance().Add(transform_, renderer_);
	parent_.AddVertexs(polInfo_);
}

void ThunderAround::ChangeState(STATE state)
{
	state_ = state;
	changeState_[state]();
}

void ThunderAround::ChangeStateReduction(void)
{
	time_ = REDUCTION_TIME;
	updateState_ = std::bind(&ThunderAround::UpdateStateReduction, this);
}

void ThunderAround::ChangeStateRotation(void)
{
	time_ = ROTATION_TIME;
	updateState_ = std::bind(&ThunderAround::UpdateStateRotation, this);
}

void ThunderAround::ChangeStateExpansion(void)
{
	time_ = EXPANSION_TIME;
	dir_ = VNorm(VSub(transform_->pos, targetPos_));
	updateState_ = std::bind(&ThunderAround::UpdateStateExpansion, this);
}

void ThunderAround::ChangeStateDead(void)
{
	updateState_ = std::bind(&ThunderAround::UpdateStateDead, this);
}

void ThunderAround::UpdateStateReduction(void)
{
	auto deltaTime = SceneManager::GetInstance().GetDeltaTime();
	transform_->pos = VAdd(transform_->pos, VScale(dir_, distance_ / (REDUCTION_TIME / deltaTime)));
	if (time_ < 0.0f)
	{
		ChangeState(STATE::ROTATION);
	}
}

void ThunderAround::UpdateStateRotation(void)
{
	rad_ += Utility::Deg2RadF(ROTATION_SPEED  * SceneManager::GetInstance().GetDeltaTime());
	float distance =static_cast<float>(Utility::Distance(targetPos_, transform_->pos));
	transform_->pos.x = targetPos_.x + distance * cosf(rad_ + initRad_);
	transform_->pos.y = targetPos_.y;
	transform_->pos.z = targetPos_.z + distance * sinf(rad_ + initRad_);
	//transform_->Update();
	if (time_ < 0.0f)
	{
		ChangeState(STATE::EXPANSION);
	}
}

void ThunderAround::UpdateStateExpansion(void)
{
	auto deltaTime = SceneManager::GetInstance().GetDeltaTime();
	transform_->pos = VAdd(transform_->pos, VScale(dir_, distance_ / (EXPANSION_TIME / deltaTime)));
	if (time_ < 0.0f)
	{
		ChangeState(STATE::DEAD);
	}
}

void ThunderAround::UpdateStateDead(void)
{
}


void ThunderAround::SetPolygonInfo(void)
{
	auto& vertexes = polInfo_.vertex;
	vertexes.clear();
	float radper = Utility::Deg2RadF(Utility::ONE_TRACK_DEG / VERTEX_NUM);
	for (int i = 0; i < (VERTEX_NUM + 1) * HEIGHT_VERTEX_NUM; i++)
	{
		VERTEX3DSHADER vertex = {};
		VECTOR pos = transform_->pos;
		pos.x += RADIUS * cosf(radper * static_cast<int>(i / static_cast<float>(HEIGHT_VERTEX_NUM)));
		//pos.y = i % HEIGHT_VERTEX_NUM == (0) ? HEIGHT : 0.0f;
		pos.y = (HEIGHT / (HEIGHT_VERTEX_NUM - 1)) * ((HEIGHT_VERTEX_NUM - 1) - (i % HEIGHT_VERTEX_NUM));
		pos.z += RADIUS * sinf(radper * static_cast<int>(i / static_cast<float>(HEIGHT_VERTEX_NUM)));
		vertex.pos = pos;
		vertex.spos = { 0.0f,0.0f,0.0f,1.0f };
		vertex.norm = VNorm(VSub(vertex.pos, transform_->pos));
		vertex.tan = { 0.0f,0.0f,0.0f };
		vertex.binorm = vertex.norm;
		vertex.dif = COLOR_U8(0, 0, 0, 0);
		vertex.spc = COLOR_U8(0, 0, 0, 0);
		//vertex.u = static_cast<float>((i / 2) % 2 == 0 ? 0.0f : 1.0f);
		//vertex.v = static_cast<float>(i % 2 == 0 ? 0.0f : 1.0f);
		//vertex.u = static_cast<float>((i / HEIGHT_VERTEX_NUM) % 2 == 0 ? 0.0f : 1.0f);
		vertex.u = static_cast<float>(1.0f / (static_cast<float>(VERTEX_NUM) / (i / HEIGHT_VERTEX_NUM)));
		vertex.v = static_cast<float>((i % HEIGHT_VERTEX_NUM) * (1.0f / (HEIGHT_VERTEX_NUM - 1)));
		vertex.su = vertex.u;
		vertex.sv = vertex.v;
		vertexes.push_back(vertex);
	}
	auto& indices = polInfo_.Indices;
	indices.clear();
	//for (int i = 0; i < VERTEX_NUM; i++)
	//{
	//	int curTop = i * 2;
	//	int curBot = curTop + 1;

	//	int nextTop = ((i + 1) % VERTEX_NUM) * 2;
	//	int nextBot = nextTop + 1;

	//	// ŽOŠpŒ`1
	//	indices.push_back(curTop);
	//	indices.push_back(nextTop);
	//	indices.push_back(curBot);

	//	// ŽOŠpŒ`2
	//	indices.push_back(nextTop);
	//	indices.push_back(nextBot);
	//	indices.push_back(curBot);
	//}
	//auto& indices = polInfo_.Indices;
	//indices.clear();
	//indices.reserve(VERTEX_NUM * (HEIGHT_VERTEX_NUM - 1) * 6);

	for (int x = 0; x < VERTEX_NUM; x++)
	{
		int nextX = (x + 1);// % VERTEX_NUM;

		for (int y = 0; y < HEIGHT_VERTEX_NUM - 1; y++)
		{
			int cur = x * HEIGHT_VERTEX_NUM + y;
			int curUp = x * HEIGHT_VERTEX_NUM + (y + 1);

			int next = nextX * HEIGHT_VERTEX_NUM + y;
			int nextUp = nextX * HEIGHT_VERTEX_NUM + (y + 1);

			// ŽOŠpŒ`1
			indices.push_back(cur);
			indices.push_back(next);
			indices.push_back(curUp);

			// ŽOŠpŒ`2
			indices.push_back(next);
			indices.push_back(nextUp);
			indices.push_back(curUp);
		}
	}


}