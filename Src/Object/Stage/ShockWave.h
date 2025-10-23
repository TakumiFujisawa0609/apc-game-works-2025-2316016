#pragma once
#include <memory>
class Transform;
class ModelMaterial;
class ModelRenderer;

class ShockWave
{
public:

	static constexpr VECTOR SIZE = {1.5f,3.0f,1.5f};
	static constexpr float RADIUS = 1500.0f;
	ShockWave(void);
	~ShockWave(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	const int GetModelId(void);
private:

	static constexpr float WAVE_SCALE = 2.0f;

	std::unique_ptr<Transform> transform_;
	std::unique_ptr<ModelMaterial> material_;
	std::unique_ptr<ModelRenderer> renderer_;

	float time_;
};

