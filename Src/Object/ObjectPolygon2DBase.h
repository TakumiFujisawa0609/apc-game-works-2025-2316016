#pragma once
#include "ObjectBase.h"
#include "../Renderer/Polygon2DMaterial.h"
#include "../Renderer/Polygon2DRenderer.h"

class ObjectPolygon2DBase :  public ObjectBase
{
public:

	ObjectPolygon2DBase() = default;
	virtual ~ObjectPolygon2DBase()override;
	virtual void Init(void)override = 0;
	virtual void Update(void)override = 0;
	virtual void Draw(void)override = 0;
	virtual void UIDraw(void)override;

	virtual void OnHit(const std::weak_ptr<Collider> _hitCol, VECTOR hitPos)override;
protected:

	Polygon2DRenderer::PolygonInfo polygonInfo_;

	std::unique_ptr<Polygon2DMaterial> material_;
	std::shared_ptr<Polygon2DRenderer> renderer_;
};

