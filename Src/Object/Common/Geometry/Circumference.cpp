#include "../../Utility/Utility.h"
#include "Triangle3D.h"
#include "Sphere.h"
#include "Line3D.h"
#include "Capsule.h"
#include "Cylinder.h"
#include "Circumference.h"

Circumference::Circumference(VECTOR& pos, float radius) : pos_(pos), radius_(radius)
{
}

Circumference::~Circumference(void)
{
}

void Circumference::Update(void)
{
}

void Circumference::Draw(void)
{
}

const bool Circumference::IsHit(Geometry& _geometry, VECTOR& _hitPos)
{
    bool ret = _geometry.IsHit(*this, _hitPos);
    return ret;
}

const bool Circumference::IsHit(Sphere& _sphere, VECTOR& _hitPos)
{
    bool ret = _sphere.IsHit(*this, _hitPos);
    return ret;
}

const bool Circumference::IsHit(Capsule& _capsule, VECTOR& _hitPos)
{
    bool ret = _capsule.IsHit(*this, _hitPos);
    return ret;
}

const bool Circumference::IsHit(Line3D& _line, VECTOR& _hitPos)
{
    bool ret = _line.IsHit(*this, _hitPos);
    return ret;
}

const bool Circumference::IsHit(Triangle3D& _triangle, VECTOR& _hitPos)
{
    bool ret = _triangle.IsHit(*this, _hitPos);
    return ret;
}

const bool Circumference::IsHit(Cylinder& _circle, VECTOR& _hitPos)
{
    bool ret = _circle.IsHit(*this, _hitPos);
    return ret;
}

const bool Circumference::IsHit(Circumference& _circle, VECTOR& _hitPos)
{
    return false;
}
