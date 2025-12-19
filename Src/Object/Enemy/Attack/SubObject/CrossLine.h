#pragma once
#include <DxLib.h>
#include "SubObjectBase.h"

class CrossAttack;

class CrossLine : public SubObjectBase
{
public:
	static constexpr float RADIUS = 70.0f; //îºåa
	static constexpr int VERTEX_NUM = 8;
	//static constexpr float MODEL_SIZE_XZ = 100.0f; //îºåa
	//static constexpr float MODEL_SIZE_Y = 900.0f; //îºåa
	//static constexpr float SIZE_XZ = RADIUS / MODEL_SIZE_XZ; //
	//static constexpr float SIZE_Y = RADIUS / MODEL_SIZE_Y * 2.0f; //
	static constexpr float DISTANCE = 200.0f;	//èâä˙ãóó£
	static constexpr float DAMAGE = 7.5f; //É_ÉÅÅ[ÉW

	//static constexpr float SIZE_Z = CrossAttack::LINE_POINT_NUM * SIZE_XZ;	//èâä˙ãóó£

	CrossLine(VECTOR centerPos,float& rad , float initRad,int num , CrossAttack& parent);
	~CrossLine(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
private:
	CrossAttack& parent_;
	float& radian_;
	VECTOR centerPos_;
	float initRadian_;
	int num_; //âΩëÃñ⁄ÇÃÇ‡ÇÃÇ©

	void SetPolygonInfo(void)override;
};

