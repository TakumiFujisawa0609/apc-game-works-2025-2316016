#include "../../Utility/Utility.h"
#include "Sphere.h"
#include "Capsule.h"
#include "Line3D.h"
#include "Cylinder.h"
#include "Circumference.h"
#include "Triangle3D.h"

Triangle3D::Triangle3D(VECTOR& pos1, VECTOR& pos2, VECTOR& pos3) :pos1_(pos1), pos2_(pos2), pos3_(pos3)
{

}

Triangle3D::~Triangle3D(void)
{
}

void Triangle3D::Update(void)
{
}

void Triangle3D::Draw(void)
{
	DrawTriangle3D(pos1_, pos2_, pos3_, DRAW_COLOR, true);
	DrawTriangle3D(pos1_, pos3_, pos2_, DRAW_COLOR, true);
}

const bool Triangle3D::IsHit(Geometry& _geometry, VECTOR& _hitPos)
{
	bool ret = _geometry.IsHit(*this, _hitPos);
	return ret;
}

const bool Triangle3D::IsHit(Sphere& _sphere, VECTOR& _hitPos)
{
	VECTOR pos = _sphere.GetPos();
	float radius = _sphere.GetRadius();
	bool ret = Utility::IsColSphere2Triangle(pos, radius, pos1_, pos2_, pos3_, _hitPos);
	return ret;
}

const bool Triangle3D::IsHit(Capsule& _capsule, VECTOR& _hitPos)
{
	return false;
}

const bool Triangle3D::IsHit(Line3D& _line, VECTOR& _hitPos)
{
	VECTOR pos1 = _line.GetPos1();
	VECTOR pos2 = _line.GetPos2();
	bool ret = Utility::IsColTriangle2Line(pos1_, pos2_, pos3_, pos1, pos2, _hitPos);
	return ret;
}

const bool Triangle3D::IsHit(Triangle3D& _triangle, VECTOR& _hitPos)
{
	return false;
}

const bool Triangle3D::IsHit(Cylinder& _circle, VECTOR& _hitPos)
{
	return false;
}

const bool Triangle3D::IsHit(Circumference& _circle, VECTOR& _hitPos)
{
	return false;
}
