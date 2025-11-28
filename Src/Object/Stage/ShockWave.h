#pragma once
#include <memory>
#include "../ObjectModelBase.h"

class ShockWave : public ObjectModelBase
{
public:

	static constexpr VECTOR SIZE = {1.5f,2.0f,1.5f};
	ShockWave(float deg);
	~ShockWave(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
private:

	static constexpr float WAVE_SCALE = 2.0f;

	float time_;
};

