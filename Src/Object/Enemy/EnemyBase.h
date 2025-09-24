#pragma once
#include <DxLib.h>
#include <memory>
#include <functional>
#include <vector>

class Transform;
class Gravity;
class AttackBase;

class EnemyBase
{
public:

	enum class STATE
	{
		IDLE,	//待機
		ATTACK, //攻撃
		DEAD,   //死亡
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name=""></param>
	EnemyBase(Transform& target);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	virtual ~EnemyBase(void);

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name=""></param>
	virtual void Init(void);

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name=""></param>
	virtual void Update(void);

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name=""></param>
	virtual void Draw(void);

	Transform& GetTransform(void) { return *transform_; }

	void Damage(float damage); //ダメージ処理

	float GetMaxHP(void) const { return maxHP_; }
	float GetHP(void) const { return hp_; }

protected:

	//基本情報
	std::unique_ptr<Transform> transform_;
	STATE state_; //状態
	std::unique_ptr<Gravity> gravity_; //重力

	Transform& target_; //ターゲット
	std::vector<AttackBase*> attackList_; //攻撃リスト
	//体力
	float maxHP_; //最大体力
	float hp_; //体力
	//移動
	//float speed_; //移動速度


private:


};

