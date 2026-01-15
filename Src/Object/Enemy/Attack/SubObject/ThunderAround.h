#pragma once
#include <memory>
#include <map>
#include <DxLib.h>
#include <functional>
#include "SubObjectBase.h"

class ThunderAroundAttack;

class ThunderAround : public SubObjectBase
{
public:

	static constexpr float DAMAGE = 7.5f;
	static constexpr float RADIUS = 40.0f;
	static constexpr float HEIGHT = 750;	//‚‚³
	static constexpr int HEIGHT_VERTEX_NUM = 4;	//‚‚³
	static constexpr int VERTEX_NUM = 16;
	static constexpr float MOVE_ADD_DISTANCE = 150.0f; //ˆÚ“®‹——£‚Ì‰ÁZ’l
	static constexpr float REDUCTION_TIME = 1.5f; //k¬ŠÔ
	static constexpr float ROTATION_TIME = 2.5f; //‰ñ“]ŠÔ
	static constexpr float EXPANSION_TIME = 1.5f; //Šg‘åŠÔ
	static constexpr float ROTATION_SPEED = 360.0f; //1•b‚Ì‰ñ“]‘¬“x
	//static constexpr float MODEL_SIZE_XZ = 100.0f;
	//static constexpr float MODEL_SCALE_XZ = RADIUS / MODEL_SIZE_XZ;


	enum class STATE
	{
		REDUCTION,	//k¬
		ROTATION,	//‰ñ“]
		EXPANSION,	//Šg‘å
		DEAD,
	};

	ThunderAround(VECTOR targetPos,VECTOR initPos, float initRad, ThunderAroundAttack& parent);
	~ThunderAround(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	bool IsDead(void) const { return state_ == STATE::DEAD; }
private:
	ThunderAroundAttack& parent_;
	//‘Šè‚Ì
	VECTOR targetPos_;
	float rad_; //’†S‚©‚ç‚ÌŠp“x
	float initRad_; //‰ŠúŠp“x
	float sumTime_;	//‡ŒvŠÔ@
	float time_;
	float distance_;
	VECTOR dir_;

	STATE state_;
	std::map<STATE, std::function<void(void)>> changeState_; //ó‘Ô•ÏX‚ÌŠÖ”Ši”[—p
	void ChangeState(STATE state);
	void ChangeStateReduction(void);
	void ChangeStateRotation(void);
	void ChangeStateExpansion(void);
	void ChangeStateDead(void);

	std::function<void(void)> updateState_; //ó‘ÔXV‚ÌŠÖ”Ši”[—p
	void UpdateStateReduction(void);
	void UpdateStateRotation(void);
	void UpdateStateExpansion(void);
	void UpdateStateDead(void);

	void SetPolygonInfo(void)override;
};

