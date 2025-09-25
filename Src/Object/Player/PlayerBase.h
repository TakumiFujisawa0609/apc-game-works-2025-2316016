#pragma once
#include <DxLib.h>
#include <memory>
#include <map>
#include <functional>
#include <vector>

class Transform;
class KeyConfig;
class PlayerShot;

class PlayerBase
{
public:

	//最大体力
	static constexpr float MAX_HP = 100.0f; 

	//移動関連
	static constexpr float MOVE_SPEED = 5.0f; //移動速度
	static constexpr VECTOR MOVE_LIMIT_MIN = { -500.0f,0.0f,-500.0f }; //移動制限最小座標
	static constexpr VECTOR MOVE_LIMIT_MAX = { 500.0f,500.0f,500.0f }; //移動制限最小座標

	//回避関連
	static constexpr float AVOID_DISTANCE = 300.0f; //回避距離
	static constexpr float AVOID_TIME = 0.3f; //回避タイム
	static constexpr float AVOID_COOL_TIME = 0.5f; //回避クールタイム

	//ダメージ関連
	static constexpr float DAMAGE_TIME = 1.0f; //ダメージ時間
	static constexpr float DAMAGE_INVINCIBLE_TIME = 3.0f; //ダメージ無敵時間

	//攻撃関連
	static constexpr float ATTACK_DELEY = 0.5f; //攻撃ディレイ

	enum class STATE
	{
		IDLE,	//待機
		MOVE,	//移動
		AVOID,  //回避
		CHARGE, //チャージ
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

	/// <summary>
	/// 状態変更
	/// </summary>
	/// <param name="state">次の状態</param>
	/// <param name="isAbsolute">同じやつでももう１度呼ぶならtrue</param>
	void ChengeState(STATE state,bool isAbsolute = false); //状態変更

protected:

	int playerNum_; //プレイヤー番号
	//基本情報
	std::unique_ptr<Transform> transform_;
	KeyConfig& keyIns_;
	//弾
	std::vector<std::unique_ptr<PlayerShot>> shots_;
	//状態
	STATE state_; 

	//回避
	VECTOR avoidDir_; //回避方向
	float avoidTime_; //回避時間
	float avoidCoolTime_; //回避クールタイム

	//攻撃
	float attackDeley_; //攻撃ディレイ

	//体力
	float hp_;

	//状態変更
	std::map<STATE, std::function<void(void)>> stateChanges_;
	void StateChangeIdle(void);   //待機
	void StateChangeMove(void);   //移動
	void StateChangeAvoid(void);  //回避
	void StateChangeCharge(void); //チャージ
	void StateChangeAttack(void); //攻撃
	void StateChangeDamage(void); //ダメージ
	void StateChangeDead(void);   //死亡

	std::function<void(void)> stateUpdate_; //状態更新関数
	void StateUpdateIdle(void);   //待機
	void StateUpdateMove(void);   //移動
	void StateUpdateAvoid(void);  //回避
	void StateUpdateCharge(void); //チャージ
	void StateUpdateAttack(void); //攻撃
	void StateUpdateDamage(void); //ダメージ
	void StateUpdateDead(void);   //死亡




	void PlayerMove(void); //移動処理

	void MoveLimit(void); //移動制限

	void SetupStateChange(void); //状態変更関数の設定

	bool IsPushMoveKey(void); //移動キーが押されているか

	void CreateShot(void); //弾の生成
private:

};
