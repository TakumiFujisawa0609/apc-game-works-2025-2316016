#pragma once
#include <vector>
#include <memory>
#include "Geometry/Geometry.h"

class ObjectBase;

class Collider
{
public:

	// 衝突用タグ
	enum class TAG
	{
		PLAYER,			//プレイヤー
		PLAYER_LAND,	//プレイヤーの地面
		PLAYER_ATTACK,	//プレイヤーの攻撃
		ENEMY,			//敵
		ENEMY_ATTACK,	//敵の攻撃
		GATE,			//ゲート
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_tags">自身の衝突用タグ</param>
	/// <param name="_geometry">当たり判定の形状</param>
	/// <param name="_notHitTags">衝突させないタグ</param>
	Collider(ObjectBase& parent, TAG tag, Geometry& geometry, std::vector<TAG> notHitTags = {});

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~Collider(void);

	/// <summary>
	/// 衝突用タグの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>衝突用タグ</returns>
	inline const TAG GetTag(void)const { return tag_; }

	/// <summary>
	/// 当たり判定の形状を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>当たり判定形状</returns>
	inline Geometry& GetGeometry(void)const { return geometry_; }

	/// <summary>
	/// 衝突させないタグの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>衝突させないタグ</returns>
	inline const std::vector<TAG> GetNotHitTags(void)const { return notHitTags_; }

	/// <summary>
	/// 親を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>親オブジェクト</returns>
	inline  ObjectBase& GetParent(void)const { return parent_; }

	/// <summary>
	/// 当たり判定の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>当たり判定(true:当たった)</returns>
	inline const bool IsHit(void)const { return isHit_; }

	/// <summary>
	/// 終了判定の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>終了判定(true:終了)</returns>
	inline const bool IsDead(void)const { return isDead_; }


	/// <summary>
	/// 終了処理(所持者の解放時に置く)
	/// </summary>
	/// <param name=""></param>
	inline void Kill(void) { isDead_ = true; }

	/// <summary>
	/// 当たった時の処理
	///	 </summary>
	// <param name="_collider">相手のコライダ</param>
	void OnHit(const std::weak_ptr<Collider> _collider,VECTOR hitPos);
private:

	//親
	ObjectBase& parent_;

	TAG tag_;

	// 衝突しないタグ
	std::vector<TAG> notHitTags_;

	//当たり判定の形状
	Geometry& geometry_;

	//当たったかの判定
	bool isHit_;


	//終了判定
	bool isDead_;
};

