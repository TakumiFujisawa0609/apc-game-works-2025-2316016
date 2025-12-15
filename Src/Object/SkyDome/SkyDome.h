#pragma once
#include "../ObjectModelBase.h"

class SkyDome :public ObjectModelBase
{
public:
	static constexpr float SCALE = 30.0f; //ÉXÉPÅ[Éã
	static constexpr float ROTAT_DEG_SEC = 10;	//1ïbÇ≈âÒÇÈó 

	SkyDome(void);
	~SkyDome(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
private:
	float time_;
};

