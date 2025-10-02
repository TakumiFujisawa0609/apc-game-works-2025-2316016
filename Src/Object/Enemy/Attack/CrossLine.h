#pragma once
#include <DxLib.h>
#include <memory>

class Transform;

class CrossLine
{
public:
	static constexpr float RADIUS = 100.0f; //‰ñ“]”¼Œa
	static constexpr float DAMAGE = 7.5f; //‰ñ“]”¼Œa

	CrossLine(VECTOR centerPos,float& rad , float initRad,int num);
	~CrossLine(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	Transform& GetTransform(void) { return *transform_; }
private:
	std::unique_ptr<Transform> transform_;
	float& radian_;
	VECTOR centerPos_;
	float initRadian_;
	int num_; //‰½‘Ì–Ú‚Ì‚à‚Ì‚©
};

