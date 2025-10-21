#pragma once
#include <memory>
class Transform;
class ModelMaterial;
class ModelRenderer;

class Floor
{
public:

	static constexpr float SIZE = 3.0f;
	static constexpr float TEXTURE_SCALE = 30.0f;
	Floor(void);
	~Floor(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	const int GetModelId(void);
private:

	std::unique_ptr<Transform> transform_;
	std::unique_ptr<ModelMaterial> material_;
	std::unique_ptr<ModelRenderer> renderer_;
};

