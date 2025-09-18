#pragma once
#include <DxLib.h>
#include <memory>

class Transform;
class Gravity;

class PlayerBase
{
public:

	static constexpr float MAX_HP = 100.0f; //最大体力
	static constexpr float MOVE_SPEED = 5.0f; //移動速度
	static constexpr VECTOR MOVE_LIMIT_MIN = { -500.0f,-500.0f,-500.0f }; //移動制限最小座標
	static constexpr VECTOR MOVE_LIMIT_MAX = { 500.0f,500.0f,500.0f }; //移動制限最小座標
	static constexpr float JUMP_POWER = 40.0f; //ジャンプ力

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
	PlayerBase(int playerNum);

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

	Transform& GetTransform(void) { return *transform_; }

protected:

	int playerNum_; //プレイヤー番号
	//基本情報
	std::unique_ptr<Transform> transform_;
	//重力
	std::unique_ptr<Gravity> gravity_;
	//場所
	PLACE place_;
	//体力
	float hp_;

	void PlayerMove(void); //移動処理
	void Jump(void); //ジャンプ処理

	void ApplyGravity(void); //重力適用

	void MoveLimit(void); //移動制限
private:

};
