#pragma once
#include <DxLib.h>
#include <memory>
#include <map>
#include <functional>
#include <vector>
#include "../../Manager/KeyConfig.h"

class Transform;
class PlayerShot;
class Gravity;

class PlayerBase
{
public:

	//最大体力
	static constexpr float MAX_HP = 100.0f; 

	//移動関連
	static constexpr float MOVE_SPEED = 5.0f; //移動速度
	static constexpr VECTOR MOVE_LIMIT_MIN = { -750.0f,0.0f,-750.0f }; //移動制限最小座標
	static constexpr VECTOR MOVE_LIMIT_MAX = { 750.0f,500.0f,750.0f }; //移動制限最小座標

	//回避関連
	static constexpr float AVOID_DISTANCE = 300.0f; //回避距離
	static constexpr float AVOID_TIME = 0.3f; //回避タイム
	static constexpr float AVOID_COOL_TIME = 0.5f; //回避クールタイム

	//ダメージ関連
	static constexpr float DAMAGE_TIME = 0.5f; //ダメージ時間
	static constexpr float DAMAGE_INVINCIBLE_TIME = 1.5f; //ダメージ無敵時間
	static constexpr float DAMAGE_SPEED = 15.0f;	//ダメージの吹っ飛びスピード

	static constexpr float JUMP_POW = 10.0f; //ジャンプ力

	//当たり判定
	static constexpr float RADIUS = 10.0f;

	//攻撃関連
	static constexpr float ATTACK_DELEY = 0.5f; //攻撃ディレイ

	enum class STATE
	{
		IDLE,	//待機
		MOVE,	//移動
		JUMP,	//ジャンプ
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
	/// <returns>成功(true)か失敗(false)か</returns>
	bool ChangeState(STATE state,bool isAbsolute = false); //状態変更

	void Damage(float damage,VECTOR dir);
	/// <summary>
	/// ダメージを食らう状態か
	/// </summary>
	/// <param name=""></param>
	/// <returns>食らう状態ならtrue</returns>
	bool IsDamageHit(void);

	STATE GetState(void) const { return state_; }
	float GetHP(void) const { return hp_; }

	int GetPlayerShotNum(void) { return static_cast<int>(shots_.size()); }
	PlayerShot& GetPlayerShot(int num) { return *shots_[num]; }
protected:

	bool isDesth_;
	KeyConfig::TYPE controlType_;
	int playerNum_; //プレイヤー番号
	//基本情報
	std::unique_ptr<Transform> transform_;
	KeyConfig& keyIns_;
	std::unique_ptr<Gravity> gravity_;
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

	//ダメージ
	float damageTime_;	//ダメージ硬直時間
	float damageInvincibleTime_;	//ダメージ後の無敵時間
	VECTOR damageDir_;	//ダメージ後の吹っ飛び方向

	//体力
	float hp_;

	//状態変更
	std::map<STATE, std::function<void(void)>> stateChanges_;
	void StateChangeIdle(void);   //待機
	void StateChangeMove(void);   //移動
	void StateChangeJump(void);   //ジャンプ
	void StateChangeAvoid(void);  //回避
	void StateChangeCharge(void); //チャージ
	void StateChangeAttack(void); //攻撃
	void StateChangeDamage(void); //ダメージ
	void StateChangeDead(void);   //死亡

	std::function<void(void)> stateUpdate_; //状態更新関数
	void StateUpdateIdle(void);   //待機
	void StateUpdateMove(void);   //移動
	void StateUpdateJump(void);   //ジャンプ
	void StateUpdateAvoid(void);  //回避
	void StateUpdateCharge(void); //チャージ
	void StateUpdateAttack(void); //攻撃
	void StateUpdateDamage(void); //ダメージ
	void StateUpdateDead(void);   //死亡



	void PlayerMove(void); //移動処理

	void MoveLimit(void); //移動制限
	void AplayGravity(void);	//重力適用

	void SetupStateChange(void); //状態変更関数の設定

	bool IsPushMoveKey(void); //移動キーが押されているか

	void CreateShot(void); //弾の生成
private:

};
