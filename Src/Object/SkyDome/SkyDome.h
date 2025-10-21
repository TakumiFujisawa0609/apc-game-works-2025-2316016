#pragma once
class SkyDome
{
public:
	static constexpr float SCALE = 30.0f; //ÉXÉPÅ[Éã

	SkyDome(void);
	~SkyDome(void);
	void Init(void);
	void Update(void);
	void Draw(void);
private:
	int modelHandle_;
};

