#include "../../Manager/ResourceManager.h"
#include "../../Manager/DrawTranslucentManager.h"
#include "Gate.h"
#include "GateMist.h"
#include "GateManager.h"

GateManager::GateManager(std::vector<SceneManager::SCENE_ID> scenes)
{
	float degInterval = Utility::ONE_TRACK_DEG / static_cast<float>(GATE_NUM);
	for (int i = 0; i < GATE_NUM; i++)
	{
		std::unique_ptr<Gate>gate;
		int size = static_cast<int>(scenes.size());
		gate = std::make_unique<Gate>(scenes[i % size],*this);
		gate->SetPos({sinf(Utility::Deg2RadF(degInterval) * i) * GATE_RADIUS,0.0f,cosf(Utility::Deg2RadF(degInterval) * i) * GATE_RADIUS });
		gate->SetDegY(degInterval * i);
		gates_.push_back(std::move(gate));
	}
	time_ = 0.0f;
	transform_ = std::make_shared<Transform>();
	transform_->pos = Utility::VECTOR_ZERO;
	transform_->Update();
	material_ = std::make_unique<Polygon3DMaterial>(
		"MistVS.cso", 0,
		"MistPS.cso", 1
	);
	material_->AddConstBufPS({ time_,0.0f,0.0f,0.0f });
	material_->AddTextureBuf(ResourceManager::GetInstance().Load(ResourceManager::SRC::TITLE_IMAGE_3).handleId_);
	material_->AddTextureBuf(ResourceManager::GetInstance().Load(ResourceManager::SRC::NOISE).handleId_);
	renderer_ = std::make_shared<Polygon3DRenderer>(*material_, polygonInfo_);
	renderer_->SetBuckCull(true);
}

GateManager::~GateManager(void)
{
}

void GateManager::Init(void)
{

}

void GateManager::Update(void)
{
	polygonInfo_.clear();
	time_ += SceneManager::GetInstance().GetDeltaTime();
	for (auto& gate : gates_)
	{
		gate->Update();
	}
	material_->SetConstBufPS(0, { time_,0.0f,0.0f,0.0f });
}

void GateManager::Draw(void)
{
	for (auto& gate : gates_)
	{
		gate->Draw();
	}
	DrawTranslucentManager::GetInstance().Add(transform_, renderer_);
}

void GateManager::AddVertexes(Polygon3DRenderer::PolygonInfo info)
{
	int vertexSize = static_cast<int>(polygonInfo_.vertex.size());
	for (auto& vertex : info.vertex)
	{
		polygonInfo_.vertex.push_back(vertex);
	}
	for (auto& indices : info.Indices)
	{
		polygonInfo_.Indices.push_back(static_cast<unsigned short>( indices + vertexSize));
	}
}
