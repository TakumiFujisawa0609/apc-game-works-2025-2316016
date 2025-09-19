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

	enum class STATE
	{
		IDLE,	//待機
		MOVE,	//移動
		AVOID,  //回避
		ATTACK, //攻撃
		DAMAGE, //ダメージ
		DEAD,   //死亡
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

	Transform& GetTransform(void) { return *transform_; }

protected:

	int playerNum_; //プレイヤー番号
	//基本情報
	std::unique_ptr<Transform> transform_;
	//体力
	float hp_;

	void PlayerMove(void); //移動処理

	void MoveLimit(void); //移動制限
private:

};
