#pragma once
#include <DxLib.h>
#include "SubObjectBase.h"

class FallDownAttack;

class FallDownShot : public SubObjectBase
{
public:
	enum class STATE
	{
		FALL,
		BLAST,
		DEAD,
	};

	static constexpr float SLOW_SPEED = 3.0f;	//Å’x‘¬“x
	static constexpr float FAST_SPEED = 10.0f;	//Å‘¬‘¬“x

	static constexpr float DAMAGE = 30.0f;		//ƒ_ƒ[ƒW
	static constexpr float RADIUS_MIN = 150.0f;	//‰~‚ÌÅ¬”¼Œa
	static constexpr float RADIUS_MAX = 300.0f;	//‰~‚ÌÅ‘å”¼Œa
	static constexpr int VERTEX_NUM = 32;		//‰~‚Ì’¸“_”
	static constexpr int ALPHA = 128;			//‰~‚Ì“§–¾“x
	static constexpr float BLAST_TIME = 0.5f;	//“–‚½‚è”»’è‚Ìd’¼ŠÔ

	FallDownShot(FallDownAttack& parent);
	~FallDownShot(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	STATE GetState(void) const { return state_; }
	float GetRadius(void) { return radius_; }
	void Hit(void);
private:
	COLOR_F color_;	//F
	float radius_;	//”¼Œa
	float initY;	//‰ŠúYÀ•W
	float blastTime_;	//d’¼ŠÔ
	STATE state_;		//ó‘Ô
	float speed_;		//—‰º‘¬“x
	float InitSpeed(void);
	VECTOR InitPos(void);

	void SetPolygonInfo(void)override;
};

