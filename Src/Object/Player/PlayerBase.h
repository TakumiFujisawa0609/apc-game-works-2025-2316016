#pragma once
#include <DxLib.h>
#include <map>
#include <functional>
#include <vector>
#include "../../Manager/KeyConfig.h"
#include "../ObjectBase.h"

class PlayerShot;
class Gravity;
class AnimationController;
class PointLight;

class PlayerBase : public ObjectBase
{
public:

	//リムライトの色
	static constexpr COLOR_F RIM_COLOR = { 1.0f,1.0f,1.0f,1.0f };
	static constexpr float RIM_MIN_POW = 10.0f;
	static constexpr float RIM_MAX_POW = 0.0f;

	//最大体力
	static constexpr float MAX_HP = 100.0f; 

	//デフォルトの色
	static constexpr COLOR_F DEFAULT_COLOR_TIMES = { 1.0f,1.0f,1.0f,1.0f };

	//移動関連
	static constexpr float MOVE_SPEED = 8.0f; //移動速度
	static constexpr VECTOR MOVE_LIMIT_MIN = { -750.0f,0.0f,-750.0f }; //移動制限最小座標
	static constexpr VECTOR MOVE_LIMIT_MAX = { 750.0f,500.0f,750.0f }; //移動制限最小座標

	//回避関連
	static constexpr float AVOID_DISTANCE = 400.0f; //回避距離
	static constexpr float AVOID_TIME = 0.6f; //回避タイム
	static constexpr float AVOID_COOL_TIME = 0.5f; //回避クールタイム

	//ダメージ関連
	static constexpr float DAMAGE_TIME = 0.5f; //ダメージ時間
	static constexpr float DAMAGE_INVINCIBLE_TIME = 1.5f; //ダメージ無敵時間
	static constexpr float DAMAGE_SPEED = 30.0f;	//ダメージの吹っ飛びスピード
	static constexpr float DAMAGE_POW = 10.0f;	//ダメージの吹っ飛びスピード
	static constexpr COLOR_F DAMAGE_COLOR_TIMES = { 1.0f,0.0f,0.0f,1.0f }; //ダメージ時の色
	static constexpr float ENEMY_HIT_DAMAGE = 5.0f;

	static constexpr float JUMP_POW = 15.0f; //ジャンプ力
	
	static constexpr float SIZE = 0.3f; //プレイヤーの大きさ

	//当たり判定
	static constexpr float RADIUS = 10.0f;
	static constexpr int HEAD_BONE_NO = 5; //頭のボーン番号

	//攻撃関連
	static constexpr float ATTACK_DELEY = 1.0f; //攻撃ディレイ

	//回復関連
	static constexpr float HEAL_PER_SEC = 3.0f;	//１秒にどれくらい回復するか
	static constexpr float DAMAGE_HEAL_DERAY = 10.0f;	//ダメージ後何秒回復しないか

	enum class STATE
	{
		IDLE,	//待機
		MOVE,	//移動
		JUMP,	//ジャンプ
		AVOID,  //回避
		//CHARGE, //チャージ
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
	~PlayerBase(void)override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name=""></param>
	virtual void Init(void)override;

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name=""></param>
	virtual void Update(void)override;

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name=""></param>
	virtual void Draw(void)override;

	virtual void UIDraw(void)override;

	virtual void OnHit(const std::weak_ptr<Collider> _hitCol, VECTOR hitPos)override;
	/// <summary>
	/// 状態変更
	/// </summary>
	/// <param name="state">次の状態</param>
	/// <param name="isAbsolute">同じやつでももう１度呼ぶならtrue</param>
	/// <returns>成功(true)か失敗(false)か</returns>
	virtual bool ChangeState(STATE state,bool isAbsolute = false) = 0; //状態変更

	void SetPos(const VECTOR& pos);
	void SetRot(const VECTOR& rot);
	STATE GetState(void) const { return state_; }
protected:

	int playerNum_; //プレイヤー番号
	//状態
	STATE state_;
	KeyConfig& keyIns_;
	std::unique_ptr<AnimationController> animCtrl_;

	std::unique_ptr<PointLight> pointLight_; //ポイントライト

	float rimPow_;

	void InitAnimationController(void); //アニメーションコントローラーの初期化
private:

};
