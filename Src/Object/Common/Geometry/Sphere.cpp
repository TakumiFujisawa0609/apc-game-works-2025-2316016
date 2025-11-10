#include "../../Utility/Utility.h"
#include "Triangle3D.h"
#include "Capsule.h"
#include "Line3D.h"
#include "Sphere.h"

Sphere::Sphere(VECTOR& pos, float radius) : pos_(pos), radius_(radius)
{
}

Sphere::~Sphere(void)
{
}

void Sphere::Update(void)
{
}

void Sphere::Draw(void)
{
	DrawSphere3D(pos_, radius_, DRAW_VERTEX_NUM, DRAW_COLOR, DRAW_COLOR, true);
}

const bool Sphere::IsHit(Geometry& _geometry, VECTOR& _hitPos)
{
	bool ret = _geometry.IsHit(*this,_hitPos);
	return ret;
}

const bool Sphere::IsHit(Sphere& _sphere, VECTOR& _hitPos)
{
	return false;
}

const bool Sphere::IsHit(Capsule& _capsule, VECTOR& _hitPos)
{
	bool ret = _capsule.IsHit(*this, _hitPos);
	return ret;
}

const bool Sphere::IsHit(Line3D& _line, VECTOR& _hitPos)
{
	VECTOR pos1 = _line.GetPos1();
	VECTOR pos2 = _line.GetPos2();
	return false;
}

const bool Sphere::IsHit(Triangle3D& _triangle, VECTOR& _hitPos)
{
	bool ret = _triangle.IsHit(*this, _hitPos);
	return ret;
}
