#include "../../Utility/Utility.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/DrawTranslucentManager.h"
#include "GateMist.h"

GateMist::GateMist(VECTOR pos[VERTEX_NUM])
{
	time_ = 0.0f;
	transform_ = std::make_shared<Transform>();
	transform_->Update();
	SetPos(pos);
	material_ = std::make_unique<Polygon3DMaterial>(
		"MistVS.cso", 0,
		"MistPS.cso", 1
	);
	material_->AddConstBufPS({ time_,0.0f,0.0f,0.0f });
	renderer_ = std::make_shared<Polygon3DRenderer>( *material_	);

	SetPolygonInfo();
}

GateMist::~GateMist(void)
{
}

void GateMist::Init(void)
{
}

void GateMist::Update(void)
{
	float delta = SceneManager::GetInstance().GetDeltaTime();
	time_ += delta;
	material_->SetConstBufPS(0,{ time_,0.0f,0.0f,0.0f });
}

void GateMist::Draw(void)
{
	renderer_->SetInfo(polygonInfo_);
	renderer_->Draw();
}

void GateMist::SetPos(VECTOR pos[VERTEX_NUM])
{
	transform_->pos = pos[0];
	for (int i = 0; i < VERTEX_NUM; i++)
	{
		pos_[i] = pos[i];
	}
}

void GateMist::SetPolygonInfo(void)
{
	VECTOR norm;
	norm = Utility::CalcNormal(pos_[0], pos_[1], pos_[2]);

	polygonInfo_.polygonNum = POLYGON_NUM;
	polygonInfo_.vertexNum = VERTEX_NUM;
	auto& vertex = polygonInfo_.vertex;
	vertex = new VERTEX3D();
	for (int i = 0; i < VERTEX_NUM; i++)
	{

		vertex[i].pos = pos_[i];
		vertex[i].norm = norm;
		vertex[i].dif = COLOR_U8();
		vertex[i].spc = COLOR_U8();
		vertex[i].u = static_cast<float>(i % 2);
		vertex[i].v = static_cast<float>(i / 2);
		vertex[i].su = vertex->u;
		vertex[i].sv = vertex->v;
	}
	polygonInfo_.Indices = new unsigned short();
	polygonInfo_.Indices[0] = 0;
	polygonInfo_.Indices[1] = 1;
	polygonInfo_.Indices[2] = 2;
	polygonInfo_.Indices[3] = 1;
	polygonInfo_.Indices[4] = 2;
	polygonInfo_.Indices[5] = 3;
}
