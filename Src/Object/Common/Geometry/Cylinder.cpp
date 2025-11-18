#include "../../Utility/Utility.h"
#include "Sphere.h"
#include "Capsule.h"
#include "Line3D.h"
#include "Triangle3D.h"
#include "Circumference.h"
#include "Cylinder.h"

Cylinder::Cylinder(VECTOR& pos, float radius) : pos_(pos), radius_(radius)
{
}

Cylinder::~Cylinder(void)
{
}

void Cylinder::Update(void)
{
}

void Cylinder::Draw(void)
{
	Utility::DrawCircle3DXZ(pos_, radius_, DRAW_VERTEX_NUM, DRAW_COLOR, true);
}

const bool Cylinder::IsHit(Geometry& _geometry, VECTOR& _hitPos)
{
	bool ret = _geometry.IsHit(*this, _hitPos);
	return ret;
}

const bool Cylinder::IsHit(Sphere& _sphere, VECTOR& _hitPos)
{
	bool ret = _sphere.IsHit(*this, _hitPos);
	return ret;
}

const bool Cylinder::IsHit(Capsule& _capsule, VECTOR& _hitPos)
{
	bool ret = _capsule.IsHit(*this, _hitPos);
	return ret;
}

const bool Cylinder::IsHit(Line3D& _line, VECTOR& _hitPos)
{
	bool ret = _line.IsHit(*this, _hitPos);
	return ret;
}

const bool Cylinder::IsHit(Triangle3D& _triangle, VECTOR& _hitPos)
{
	bool ret = _triangle.IsHit(*this, _hitPos);
	return ret;
}

const bool Cylinder::IsHit(Cylinder& _circle, VECTOR& _hitPos)
{
	VECTOR pos = _circle.GetPos();
	float radius = _circle.GetRadius();
	bool ret =Utility::IsColCylinder2Cylinder(pos_, radius_, pos, radius, _hitPos);
	return ret;
}

const bool Cylinder::IsHit(Circumference& _circle, VECTOR& _hitPos)
{
	VECTOR pos = _circle.GetPos();
	float radius = _circle.GetRadius();
	bool ret = Utility::IsColCircumference2Circle(pos_, radius_, pos, radius, _hitPos);
	return ret;
}
