#pragma once
#include "../ObjectBase.h"
class Transform;
class ModelMaterial;
class ModelRenderer;

class Floor : public ObjectBase
{
public:

	static constexpr float SIZE = 3.0f;
	static constexpr float TEXTURE_SCALE = 30.0f;
	Floor(void);
	~Floor(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
private:

};

