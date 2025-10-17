#pragma once
#include <memory>
class Transform;
class ModelMaterial;
class ModelRenderer;
class Stage
{
public:

	static constexpr float RADIUS = 1250.0f;	//îºåa
	static constexpr int VERTEX_NUM = 360;	//í∏ì_êî
	static constexpr float SIZE = 1.25f;
	Stage(void);
	~Stage(void);
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

