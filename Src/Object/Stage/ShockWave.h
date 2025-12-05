#pragma once
#include <memory>
#include "../ObjectPolygonBase.h"

class ShockWave : public ObjectPolygonBase
{
public:
	static constexpr int WAVE_VERTEX_NUM = 60; //ショックウェーブの頂点数
	static constexpr float HEIGHT = 100.0f;	//高さ
	//static constexpr VECTOR SIZE = {1.5f,2.0f,1.5f};
	ShockWave(void);
	~ShockWave(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
private:

	static constexpr float WAVE_SCALE = 2.0f;

	float time_;
	void SetPolInfo(void);
};

