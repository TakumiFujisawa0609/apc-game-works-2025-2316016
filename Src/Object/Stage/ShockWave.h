#pragma once
#include <memory>
#include "../ObjectBase.h"
class Transform;
class ModelMaterial;
class ModelRenderer;

class ShockWave : public ObjectBase
{
public:

	static constexpr VECTOR SIZE = {1.5f,3.0f,1.5f};
	static constexpr float RADIUS = 1500.0f;
	ShockWave(void);
	~ShockWave(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
private:

	static constexpr float WAVE_SCALE = 2.0f;

	float time_;
};

