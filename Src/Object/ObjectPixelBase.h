#pragma once
#include "ObjectBase.h"
#include "../Renderer/PixelMaterial.h"
#include "../Renderer/PixelRenderer.h"

class ObjectPixelBase : public ObjectBase
{
public:

	ObjectPixelBase() = default;
	virtual ~ObjectPixelBase()override;
	virtual void Init(void)override = 0;
	virtual void Update(void)override = 0;
	virtual void Draw(void)override = 0;
	virtual void UIDraw(void)override;

	virtual void OnHit(const std::weak_ptr<Collider> _hitCol, VECTOR hitPos)override;
protected:

	// ポストエフェクト用(ビネット)
	std::unique_ptr<PixelMaterial> uiMaterial_;
	std::unique_ptr<PixelRenderer> uiRenderer_;
};

