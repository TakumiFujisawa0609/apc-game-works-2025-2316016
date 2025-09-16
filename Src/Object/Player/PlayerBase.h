#pragma once
#include <DxLib.h>
#include <memory>

class Transform;
class Gravity;

class PlayerBase
{
public:

	static constexpr float MAX_HP = 100.0f; //最大体力

	enum class PLACE
	{
		LAND,	//地上
		AIR,	//空中
	};

	enum class STATE
	{
		IDLE,	//待機
		MOVE,	//移動
		JUMP,	//ジャンプ
		ARROW_CHARGE, //弓構え
		ARROW_SHOT,   //弓発射
		DAMAGE, //ダメージ
		DEAD,   //死亡
	};

	enum class ATTACK_TYPE
	{
		NONE,	//攻撃なし
		ARROW,	//弓
		FALL_KICK, //落下キック
	};
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name=""></param>
	PlayerBase(void);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~PlayerBase(void);

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

	/// <summary>
	/// 重力方向を変える処理
	/// </summary>
	/// <param name=""></param>
	virtual void ChengeGravityDir(void);

protected:

	//基本情報
	std::unique_ptr<Transform> transform_;
	//重力
	std::unique_ptr<Gravity> gravity_;
	//場所
	PLACE place_;
	//体力
	float hp_;

private:

};

