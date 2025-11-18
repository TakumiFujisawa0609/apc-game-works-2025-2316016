#pragma once
#include "../ObjectBase.h"

class PointLight;

class Floor : public ObjectBase
{
public:

	static constexpr float SIZE = 3.0f;
	static constexpr float TEXTURE_SCALE = 30.0f;

	static constexpr COLOR_F LIGHT_COLOR = { 1.0f,1.0f,1.0f,1.0f };

	Floor(PointLight& light);
	~Floor(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
private:
	PointLight& light_;
};

