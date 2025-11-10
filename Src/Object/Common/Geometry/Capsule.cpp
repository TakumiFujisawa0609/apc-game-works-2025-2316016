#include "../../Utility/Utility.h"
#include "Triangle3D.h"
#include "Sphere.h"
#include "Line3D.h"
#include "Capsule.h"

Capsule::Capsule(VECTOR& pos1, VECTOR& pos2, float radius) : pos1_(pos1), pos2_(pos2), radius_(radius)
{
}

Capsule::~Capsule(void)
{
}

void Capsule::Update(void)
{
}

void Capsule::Draw(void)
{
	DrawCapsule3D(pos1_, pos2_, radius_, DRAW_VERTEX_NUM, DRAW_COLOR, DRAW_COLOR, true);
}

const bool Capsule::IsHit(Geometry& _geometry, VECTOR& _hitPos)
{
	bool ret = _geometry.IsHit(*this,_hitPos);
	return ret;
}

const bool Capsule::IsHit(Sphere& _sphere, VECTOR& _hitPos)
{
	VECTOR pos = _sphere.GetPos();
	float radius = _sphere.GetRadius();
	bool ret = Utility::IsColCapsule2Sphere(pos1_, pos2_, radius_, pos, radius, _hitPos);
	return ret;
}

const bool Capsule::IsHit(Capsule& _capsule, VECTOR& _hitPos)
{
	return false;
}

const bool Capsule::IsHit(Line3D& _line, VECTOR& _hitPos)
{
	VECTOR pos1 = _line.GetPos1();
	VECTOR pos2 = _line.GetPos2();
	bool ret = Utility::IsColCapsule2Line(pos1_, pos2_, radius_, pos1, pos2, _hitPos);
	return ret;
}

const bool Capsule::IsHit(Triangle3D& _triangle, VECTOR& _hitPos)
{
	return false;
}


