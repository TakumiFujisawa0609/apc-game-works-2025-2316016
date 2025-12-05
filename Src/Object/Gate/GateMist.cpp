#include "../../Utility/Utility.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/DrawTranslucentManager.h"
#include "GateManager.h"
#include "GateMist.h"

GateMist::GateMist(VECTOR pos[VERTEX_NUM], GateManager& manager):manager_(manager)
{
	SetPos(pos);
}

GateMist::~GateMist(void)
{
}

void GateMist::Init(void)
{
}

void GateMist::Update(void)
{
}

void GateMist::Draw(void)
{
	manager_.AddVertexes(polygonInfo_);
}

void GateMist::SetPos(VECTOR pos[VERTEX_NUM])
{
	for (int i = 0; i < VERTEX_NUM; i++)
	{
		pos_[i] = pos[i];
	}
	SetPolygonInfo();
}

void GateMist::SetPolygonInfo(void)
{
	VECTOR center;
	center = Utility::CalcCenter(pos_[0], pos_[1], pos_[2],pos_[3]);

	auto& vertexes = polygonInfo_.vertex;
	vertexes.clear();
	for (int i = 0; i < VERTEX_NUM; i++)
	{
		VERTEX3DSHADER vertex = {};
		vertex.pos = pos_[i];
		vertex.spos = {0.0f,0.0f,0.0f,1.0f};
		vertex.norm = VNorm(VSub(vertex.pos,center));
		vertex.tan = {0.0f,0.0f,0.0f};
		vertex.binorm = vertex.norm;
		vertex.dif = COLOR_U8(0,0,0,0);
		vertex.spc = COLOR_U8(0,0,0,0);
		vertex.u = static_cast<float>(i % 2);
		vertex.v = static_cast<float>(i / 2);
		vertex.su = vertex.u;
		vertex.sv = vertex.v;
		vertexes.push_back(vertex);
	}
	auto& indices = polygonInfo_.Indices;
	indices.clear();
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(3);
	indices.push_back(2);
}
