#pragma once
#include <DxLib.h>
#include "SubObjectBase.h"

class CrossLine : public SubObjectBase
{
public:
	static constexpr float RADIUS = 70.0f; //‰ñ“]”¼Œa
	static constexpr float DAMAGE = 7.5f; //‰ñ“]”¼Œa

	CrossLine(VECTOR centerPos,float& rad , float initRad,int num);
	~CrossLine(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
private:
	float& radian_;
	VECTOR centerPos_;
	float initRadian_;
	int num_; //‰½‘Ì–Ú‚Ì‚à‚Ì‚©
};

