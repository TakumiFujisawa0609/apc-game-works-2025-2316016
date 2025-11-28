#pragma once
#include "../ObjectPolygonBase.h"

class GateMist : public ObjectPolygonBase
{
public:

	static constexpr int VERTEX_NUM = 4;
	static constexpr int POLYGON_NUM = 2;

	GateMist(VECTOR pos[VERTEX_NUM]);
	~GateMist(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	void SetPos(VECTOR pos[VERTEX_NUM]);
private:
	float time_;
	VECTOR pos_[VERTEX_NUM];
	void SetPolygonInfo(void);
};