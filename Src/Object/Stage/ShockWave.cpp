#include "../../Utility/Utility.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/DrawTranslucentManager.h"
#include "Stage.h"
#include "ShockWave.h"

ShockWave::ShockWave(void)
{
	time_ = 0.0f;
	transform_ = std::make_shared<Transform>();
	transform_->pos = Utility::VECTOR_ZERO;
	//transform_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::SHOCK_WAVE1));
	//float rad = Utility::Deg2RadF(deg);
	//transform_->pos = { sinf(rad) * Stage::RADIUS,-10.0f,cosf(rad) * Stage::RADIUS};
	//transform_->quaRot = Quaternion(VGet(0.0f, rad, 0.0f));
	//transform_->quaRotLocal = Quaternion(VGet(0.0f, Utility::Deg2RadF(90.0f), 0.0f));
	//transform_->scl = SIZE;
	//transform_->Update();

	material_ = std::make_unique<Polygon3DMaterial>(
		"ShockWaveVS.cso", 0,
		"ShockWavePS.cso", 1
	);
	//material_->AddConstBufVS({ time_, WAVE_SCALE, 1.0f, 1.0f });
	material_->AddConstBufPS({ time_, TIME_SCALE, 1.0f, 1.0f });
	material_->AddTextureBuf(ResourceManager::GetInstance().Load(ResourceManager::SRC::WAVE_TEXTURE).handleId_);
	material_->AddTextureBuf(ResourceManager::GetInstance().Load(ResourceManager::SRC::NOISE).handleId_);
	renderer_ = std::make_shared<Polygon3DRenderer>(
		 *material_,polygonInfo_
	);
	renderer_->SetBuckCull(true);
	SetPolInfo();
}

ShockWave::~ShockWave(void)
{
}

void ShockWave::Init(void)
{
}

void ShockWave::Update(void)
{
	time_ += SceneManager::GetInstance().GetDeltaTime();
	//material_->SetConstBufVS(0, { time_, WAVE_SCALE, 1.0f, 1.0f });
	material_->SetConstBufPS(0, { time_, TIME_SCALE, 1.0f, 1.0f });
}

void ShockWave::Draw(void)
{
	//Utility::DrawCircle3DXZ(VGet(0.0f, 0.0f, 0.0f), RADIUS, VERTEX_NUM, 0xff00ff, false);
	//MV1DrawModel(transform_->modelId);
	//renderer_->Draw();
	DrawTranslucentManager::GetInstance().Add(transform_, renderer_);
}

void ShockWave::SetPolInfo(void)
{
	const float RADIUS = Stage::RADIUS;
	auto& vertexes = polygonInfo_.vertex;
	vertexes.clear();
	float radper = Utility::Deg2RadF(Utility::ONE_TRACK_DEG / WAVE_VERTEX_NUM);
	for (int i = 0; i < WAVE_VERTEX_NUM * 2; i++)
	{
		VERTEX3DSHADER vertex = {};
		VECTOR pos = transform_->pos;
		pos.x += RADIUS * cosf(radper * static_cast<int>(i / static_cast<float>(2)));
		pos.y = i % 2 == 0 ? HEIGHT : 0.0f;
		pos.z += RADIUS * sinf(radper * static_cast<int>(i / static_cast<float>(2)));
		vertex.pos = pos;
		vertex.spos = { 0.0f,0.0f,0.0f,1.0f };
		vertex.norm = VNorm(VSub(vertex.pos, transform_->pos));
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
	auto& indices = polygonInfo_.Indices;
	indices.clear();
	for (int i = 0; i < WAVE_VERTEX_NUM; i++)
	{
		int curTop = i * 2;
		int curBot = curTop + 1;

		int nextTop = ((i + 1) % WAVE_VERTEX_NUM) * 2;
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
