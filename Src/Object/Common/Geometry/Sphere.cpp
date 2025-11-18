#include "../../Utility/Utility.h"
#include "Triangle3D.h"
#include "Capsule.h"
#include "Line3D.h"
#include "Cylinder.h"
#include "Circumference.h"
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
	VECTOR pos = _sphere.GetPos();
	float radius = _sphere.GetRadius();
	bool ret = Utility::IsColSphere2Sphere(pos_, radius_, pos, radius,_hitPos);
	return ret;
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

const bool Sphere::IsHit(Cylinder& _circle, VECTOR& _hitPos)
{

	VECTOR pos = _circle.GetPos();
	float radius = _circle.GetRadius();
	bool ret = Utility::IsColCylinder2Cylinder(pos_, radius_, pos, radius, _hitPos);
	return ret;
}

const bool Sphere::IsHit(Circumference& _circle, VECTOR& _hitPos)
{
	VECTOR pos = _circle.GetPos();
	float radius = _circle.GetRadius();
	bool ret = Utility::IsColCircumference2Circle(pos, radius, pos_, radius_, _hitPos);
	return ret;
}
