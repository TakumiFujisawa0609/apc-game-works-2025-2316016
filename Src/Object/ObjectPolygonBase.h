#pragma once
#include "ObjectBase.h"
#include "../Renderer/Polygon3DMaterial.h"
#include "../Renderer/Polygon3DRenderer.h"

class ObjectPolygonBase : public ObjectBase
{
public:

	ObjectPolygonBase() = default;
	virtual ~ObjectPolygonBase()override;
	virtual void Init(void)override = 0;
	virtual void Update(void)override = 0;
	virtual void Draw(void)override = 0;
	virtual void UIDraw(void)override;

	virtual void OnHit(const std::weak_ptr<Collider> _hitCol, VECTOR hitPos)override;
protected:

	Polygon3DRenderer::PolygonInfo polygonInfo_;

	std::unique_ptr<Polygon3DMaterial> material_;
	std::shared_ptr<Polygon3DRenderer> renderer_;
};

