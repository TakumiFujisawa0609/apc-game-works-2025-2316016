#pragma once
#include "../ObjectPolygon2DBase.h"

class PushKeyUI : public ObjectPolygon2DBase
{
public:
	enum class TYPE
	{
		TITLE,
		GAMECLEAR,
		GAMEOVER,
	};

	static constexpr float TIME_POWER = 2.0f;

	PushKeyUI(FLOAT3 color,TYPE type,VECTOR outlineCol,FLOAT2 size);
	~PushKeyUI(void) override;
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void SetPolygonInfo(const Polygon2DRenderer::PolygonInfo& info) { polygonInfo_ = info; }
private:
	int uiHandle_;
	float time_ = 0.0f;
	FLOAT2 size_;
};

