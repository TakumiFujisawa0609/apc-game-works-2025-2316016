#pragma once
#include <DxLib.h>
#include "../../Object/ObjectBase.h"
class Gravity;

class ShotGuide : public ObjectBase
{
public:
	ShotGuide(VECTOR pPos, VECTOR tPos);
	~ShotGuide();
	void Init(void);
	void Update(void);
	void Draw(void);
private:
	
};

