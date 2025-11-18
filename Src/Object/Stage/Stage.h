#pragma once
#include <memory>
#include "../ObjectBase.h"
#include "ShockWave.h"
#include "Floor.h"

class PointLight;

class Stage : public ObjectBase
{
public:
	static constexpr int WAVE_VERTEX_NUM = 60; //ショックウェーブの頂点数
	static constexpr float RADIUS = 1500.0f;

	Stage(void) = default;
	Stage(PointLight& light);
	~Stage(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
private:
	std::unique_ptr<Floor> floor_;
	std::unique_ptr<ShockWave> shockWave_[WAVE_VERTEX_NUM];
};

