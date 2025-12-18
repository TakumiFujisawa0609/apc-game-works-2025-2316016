#pragma once
#include "../ObjectModelBase.h"

class SkyDome :public ObjectModelBase
{
public:
	static constexpr float SCALE = 30.0f; //ÉXÉPÅ[Éã
	static constexpr float ROTAT_DEG_SEC = 10;	//1ïbÇ≈âÒÇÈó 
	static constexpr FLOAT4 DEFAULT_COLOR = { 1.0f,1.0f,1.0f,1.0f };

	SkyDome(void);
	~SkyDome(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	void SetColor(FLOAT4 color);
private:
	float time_;
};

