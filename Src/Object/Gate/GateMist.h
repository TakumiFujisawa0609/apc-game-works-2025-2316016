#pragma once
#include "../ObjectPolygonBase.h"

class GateManager;

class GateMist : public ObjectPolygonBase
{
public:

	static constexpr int VERTEX_NUM = 4;

	GateMist(VECTOR pos[VERTEX_NUM],GateManager& manager);
	~GateMist(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	void SetPos(VECTOR pos[VERTEX_NUM]);
private:
	GateManager& manager_;
	//int noiseImg_;
	float time_;
	VECTOR pos_[VERTEX_NUM];
	void SetPolygonInfo(void);
};