#pragma once
#include <vector>
#include <memory>

class AnimationController;
class AttackBase;
class Transform;
class Gravity;
class EnemyBase;

class EnemyAttackManager
{
public:

	enum class ATTACK_TYPE	//UŒ‚‚Ìí—Ş
	{
		JUMP,		//jampUŒ‚(”g “¯)
		JUMP_CONSTANT, //jampUŒ‚(”g ˜A‘±)
		FOLLOW,		//’Ç”öUŒ‚(’Ç]’e)
		FALL_DOWN,	//—‰ºUŒ‚(—‰º’e)
		CROSS_LINE,	//\š	
		THUNDER_AROUND, //üˆÍ—‹
		WATER_SPRIT,	//…‚µ‚Ô‚«
		MAX
	};

	enum class ATTACK_STATE
	{
		READY,	//€”õ
		PLAY,	//Às
	};

	EnemyAttackManager(EnemyBase& enemy);
	~EnemyAttackManager(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void AddAttack(ATTACK_TYPE type);	//UŒ‚‚ğ’Ç‰Á
	void DeleteAttack(ATTACK_TYPE type); //UŒ‚‚ğíœ
	void AllDeleteAttack(void); //‘S‚Ä‚ÌUŒ‚‚ğíœ
	int GetAnimNumber(ATTACK_STATE state, ATTACK_TYPE type);
	int GetAttackNum(void) { return static_cast<int>(attackList_.size()); }
	bool CheckMove(void);//‰Ò“­’†‚Ì‚à‚Ì‚ª‚ ‚é‚©
	void PlayAttack(float dis);	//‹——£‚É‰‚¶‚Ä‚P‚ÂUŒ‚‚ğ‘I‚ÑÀs‚·‚é
	AnimationController& GetAnimController(void);
	std::weak_ptr<Transform> GetTransform(void);
	Gravity& GetGravity(void);
protected:
	EnemyBase& enemy_;
	std::weak_ptr<Transform> target_; //ƒ^[ƒQƒbƒg
	std::vector<std::unique_ptr<AttackBase>> attackList_; //UŒ‚ƒŠƒXƒg
private:
};

