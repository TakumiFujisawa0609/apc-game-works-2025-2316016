#pragma once
#include "ObjectBase.h"
#include "../Renderer/ModelMaterial.h"
#include "../Renderer/ModelRenderer.h"
class ObjectModelBase : public ObjectBase
{
public:

	ObjectModelBase() = default;
	virtual ~ObjectModelBase()override;
	virtual void Init(void)override = 0;
	virtual void Update(void)override = 0;
	virtual void Draw(void)override = 0;
	virtual void UIDraw(void)override;

	virtual void OnHit(const std::weak_ptr<Collider> _hitCol, VECTOR hitPos)override;
protected:

	std::unique_ptr<ModelMaterial> material_;
	std::shared_ptr<ModelRenderer> renderer_;
};

