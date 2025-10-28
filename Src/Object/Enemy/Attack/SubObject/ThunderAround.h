#pragma once
#include <memory>
#include <map>
#include <DxLib.h>
#include <functional>

class Transform;

class ThunderAround
{
public:

	static constexpr float DAMAGE = 7.5f;
	static constexpr float RADIUS = 40.0f;
	static constexpr int VERTEX_NUM = 32;
	static constexpr float MOVE_ADD_DISTANCE = 150.0f; //ˆÚ“®‹——£‚Ì‰ÁZ’l
	static constexpr float REDUCTION_TIME = 1.5f; //k¬ŠÔ
	static constexpr float ROTATION_TIME = 5.0f; //‰ñ“]ŠÔ
	static constexpr float EXPANSION_TIME = 1.5f; //Šg‘åŠÔ
	static constexpr float ROTATION_SPEED = 360.0f; //1•b‚Ì‰ñ“]‘¬“x

	enum class STATE
	{
		REDUCTION,	//k¬
		ROTATION,	//‰ñ“]
		EXPANSION,	//Šg‘å
		DEAD,
	};

	ThunderAround(VECTOR targetPos,VECTOR initPos, float initRad);
	~ThunderAround(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	Transform& GetTransform(void) { return *transform_; }
	bool IsDead(void) const { return state_ == STATE::DEAD; }
private:

	//©•ª‚Ì
	std::unique_ptr<Transform> transform_;
	//‘Šè‚Ì
	VECTOR targetPos_;
	float rad_; //’†S‚©‚ç‚ÌŠp“x
	float initRad_; //‰ŠúŠp“x

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
};

