#include "../../Utility/Utility.h"
#include "Triangle3D.h"
#include "Sphere.h"
#include "Capsule.h"
#include "Line3D.h"

Line3D::Line3D(VECTOR& pos1, VECTOR& pos2):pos1_(pos1),pos2_(pos2)
{
}

Line3D::~Line3D(void)
{
}

void Line3D::Update(void)
{
}

void Line3D::Draw(void)
{
	DrawLine3D(pos1_, pos2_, DRAW_COLOR);
}

const bool Line3D::IsHit(Geometry& _geometry, VECTOR& _hitPos)
{
	bool ret = _geometry.IsHit(*this,_hitPos);
	return ret;
}

const bool Line3D::IsHit(Sphere& _sphere, VECTOR& _hitPos)
{
	bool ret = _sphere.IsHit(*this, _hitPos);
	return false;
}

const bool Line3D::IsHit(Capsule& _capsule, VECTOR& _hitPos)
{
	bool ret = _capsule.IsHit(*this, _hitPos);
	return ret;
}

const bool Line3D::IsHit(Line3D& _line, VECTOR& _hitPos)
{
	return false;
}

const bool Line3D::IsHit(Triangle3D& _triangle, VECTOR& _hitPos)
{
	bool ret = _triangle.IsHit(*this, _hitPos);
	return ret;
}
