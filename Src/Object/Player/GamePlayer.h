#pragma once
#include "PlayerBase.h"

class GamePlayer :   public PlayerBase
{
public:

	GamePlayer(int playerNum);
	~GamePlayer(void) override;
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void UIDraw(void) override;
	void OnHit(const std::weak_ptr<Collider> _hitCol, VECTOR hitPos) override;
	bool ChangeState(STATE state, bool isAbsolute = false) override; //状態変更

	const VECTOR& GetPrePos(void)const { return prePos_; }

	void Damage(float damage, VECTOR dir);

	/// <summary>
	/// ダメージを食らう状態か
	/// </summary>
	/// <param name=""></param>
	/// <returns>食らう状態ならtrue</returns>
	bool IsDamageHit(void);

	float GetHP(void) const { return hp_; }

	int GetPlayerShotNum(void) { return static_cast<int>(shots_.size()); }
	PlayerShot& GetPlayerShot(int num) { return *shots_[num]; }
	Gravity& GetGravity(void) { return *gravity_; }
	std::vector<VECTOR> GetCollisionSpherePositions(void);	//当たり判定用の球の位置を取得

	PointLight& GetPointLight(void) { return *pointLight_; }

	void SetEnemyTransform(std::weak_ptr<Transform> enemyTrans) { enemyTrans_ = enemyTrans; }

	const float GetSumDamage(void)const { return sumDamage_; }	//総ダメージ量取得
	const int GetDamageNum(void)const { return damageNum_; }	//ダメージ回数
	const int GetAvoidNum(void)const { return avoidNum_; }	//回避回数
	const int GetAvoidSaccessNum(void)const { return avoidSaccessNum_; }	//回避成功回数
	const int GetJumpNum(void)const { return jumpNum_; }	//ジャンプ回数
	const int GetJumpSaccessNum(void)const { return jumpSaccessNum_; }	//ジャンプ回避成功回数

protected:
	int footNum_; //足音の数
	std::weak_ptr<Transform> enemyTrans_; //敵の座標

	VECTOR headPos_;	//頭座標
	VECTOR prePos_;
	VECTOR landPos_; //着地座標
	bool isDeath_;
	KeyConfig::TYPE controlType_;
	//基本情報
	std::unique_ptr<Gravity> gravity_;
	//弾
	std::vector<std::unique_ptr<PlayerShot>> shots_;

	//回避
	VECTOR avoidDir_; //回避方向
	float avoidTime_; //回避時間
	float avoidCoolTime_; //回避クールタイム
	bool isAvoidSaccess_; //回避成功したか
	float avoidSaccessTime_; //回避成功した時間
	int avoidNum_;	//ロール回避回数
	int avoidSaccessNum_;	//ロール回避成功数
	int jumpNum_;	//ジャンプ回避回数
	int jumpSaccessNum_;	//ジャンプ回避成功回数

	//攻撃
	float attackDeley_; //攻撃ディレイ

	//ダメージ
	float damageTime_;	//ダメージ硬直時間
	float damageInvincibleTime_;	//ダメージ後の無敵時間
	VECTOR damageDir_;	//ダメージ後の吹っ飛び方向
	float sumDamage_;	//合計ダメージ
	int damageNum_;	//ダメージを受けた回数

	//体力
	float hp_;

	//回復ディレイ
	float healDeray_;


	//状態変更
	std::map<STATE, std::function<void(void)>> stateChanges_;
	void StateChangeIdle(void);   //待機
	void StateChangeMove(void);   //移動
	void StateChangeJump(void);   //ジャンプ
	void StateChangeAvoid(void);  //回避
	//void StateChangeCharge(void); //チャージ
	void StateChangeAttack(void); //攻撃
	void StateChangeDamage(void); //ダメージ
	void StateChangeDead(void);   //死亡

	std::function<void(void)> stateUpdate_; //状態更新関数
	void StateUpdateIdle(void);   //待機
	void StateUpdateMove(void);   //移動
	void StateUpdateJump(void);   //ジャンプ
	void StateUpdateAvoid(void);  //回避
	//void StateUpdateCharge(void); //チャージ
	void StateUpdateAttack(void); //攻撃
	void StateUpdateDamage(void); //ダメージ
	void StateUpdateDead(void);   //死亡



	void PlayerMove(void); //移動処理

	void MoveLimit(void); //移動制限
	void AplayGravity(void);	//重力適用

	void SetupStateChange(void); //状態変更関数の設定

	bool IsPushMoveKey(void); //移動キーが押されているか

	void CreateShot(void); //弾の生成

	void Rotation(void); //プレイヤーの向き調整

	void Heal(void);

	void PlayFootSound(void); //足音再生
	void StopFootSound(void); //足音停止
	bool IsPlayFootSound(void); //足音再生中か

	void SaccessAvoid(void); //回避成功時の処理
	void SaccessJumpAvoid(void); //ジャンプ成功時の処理
};

