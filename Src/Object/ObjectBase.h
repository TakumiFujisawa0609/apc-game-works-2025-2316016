#pragma once
#include <memory>
class Transform;
class ModelMaterial;
class ModelRenderer;
class EffectController;

class ObjectBase
{
public:
	ObjectBase() = default;
	virtual ~ObjectBase() = default;
	virtual void Init(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	Transform& GetTransform(void) const{ return *transform_; }
protected:
	std::unique_ptr<Transform> transform_;
	std::unique_ptr<ModelMaterial> material_;
	std::unique_ptr<ModelRenderer> renderer_;
	std::unique_ptr<EffectController> effect_;
};

