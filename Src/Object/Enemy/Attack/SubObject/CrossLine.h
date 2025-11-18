#pragma once
#include <DxLib.h>
#include "SubObjectBase.h"
#include "../Type//CrossAttack.h"

class CrossLine : public SubObjectBase
{
public:
	static constexpr float RADIUS = 70.0f; //”¼Œa
	static constexpr float MODEL_SIZE_XZ = 100.0f; //”¼Œa
	static constexpr float MODEL_SIZE_Y = 900.0f; //”¼Œa
	static constexpr float SIZE_XZ = RADIUS / MODEL_SIZE_XZ; //
	static constexpr float SIZE_Y = RADIUS / MODEL_SIZE_Y * 2.0f; //
	static constexpr float DISTANCE = 200.0f;	//‰Šú‹——£
	static constexpr float DAMAGE = 7.5f; //ƒ_ƒ[ƒW

	//static constexpr float SIZE_Z = CrossAttack::LINE_POINT_NUM * SIZE_XZ;	//‰Šú‹——£

	CrossLine(VECTOR centerPos,float& rad , float initRad,int num);
	~CrossLine(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
private:
	float time_;
	float& radian_;
	VECTOR centerPos_;
	float initRadian_;
	int num_; //‰½‘Ì–Ú‚Ì‚à‚Ì‚©
};

