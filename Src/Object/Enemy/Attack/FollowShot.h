#pragma once
#include <memory>
#include <DxLib.h>
class Transform;

class FollowShot
{
public:
	enum class SPEED_TYPE
	{
		SLOW,
		MIDIUM,
		FAST,
		RANDOM,
		MAX,
	};

	enum class STATE
	{
		SHOT,
		BLAST,
		DEAD,
	};

	static constexpr float SLOW_SPEED = 3.0f;
	static constexpr float MIDIUM_SPEED = 5.5f;
	static constexpr float FAST_SPEED = 7.0f;

	static constexpr float DAMAGE = 15.0f;
	static constexpr float ATTACK_TIME = 10.0f;

	FollowShot(Transform& target, SPEED_TYPE speed,VECTOR startPos);
	~FollowShot(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	bool IsShot(void) { return state_ == STATE::SHOT; }
	Transform& GetTransform(void) { return *transform_; }
	void Hit(void);
private:
	//Ž©•ª‚Ì
	std::unique_ptr<Transform> transform_;
	//‘ŠŽè‚Ì
	Transform& target_;

	STATE state_;
	float speed_;
	float time_;

	float InitSpeed(SPEED_TYPE speedType) const;

};