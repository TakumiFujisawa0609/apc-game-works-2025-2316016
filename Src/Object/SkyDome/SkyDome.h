#pragma once
#include "../ObjectBase.h"

class SkyDome :public ObjectBase
{
public:
	static constexpr float SCALE = 30.0f; //ÉXÉPÅ[Éã

	SkyDome(void);
	~SkyDome(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
private:
};

