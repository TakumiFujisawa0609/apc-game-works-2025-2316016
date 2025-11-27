#pragma once
#include<vector>
#include<memory>
#include<map>
#include<functional>
#include"../../Object/Common/Collider.h"

class Geometry;

class CollisionManager
{
public:
	/// <summary>
	/// インスタンス生成
	/// </summary>
	/// <param name=""></param>
	static void CreateInstance(void);

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>インスタンス</returns>
	static CollisionManager& GetInstance(void) { return *instance_; }

	/// <summary>
	/// インスタンス破棄（メモリ解放）
	/// </summary>
	static void Destroy(void);

	/// <summary>
	/// コライダの追加
	/// </summary>
	/// <param name="_collider">コライダ</param>
	void AddCollider(const std::shared_ptr<Collider> _collider);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name=""></param>
	void Update(void);

private:

	//静的インスタンス
	static CollisionManager* instance_;

	//当たり判定格納
	std::vector<std::shared_ptr<Collider>>colliders_;

	//コンストラクタ
	CollisionManager(void);

	//コピーの禁止
	CollisionManager(const CollisionManager& _copy) = delete;
	CollisionManager& operator= (const CollisionManager& _copy) = delete;

	//デストラクタ
	~CollisionManager(void);

	/// <summary>
	/// 当たり判定をするか(全部当てはまったらtrue)
	/// </summary>
	/// <param name="_col1">1つ目のコライダ番号</param>
	/// <param name="_col2">2つ目のコライダ番号</param>
	/// <returns>true:当たり判定をする</returns>
	const bool JudgeIsCollision(const int _col1Num, const int _col2Num)const;

	/// <summary>
	/// 当たり判定
	/// </summary>
	/// <param name="_col1">1つ目のコライダ</param>
	/// <param name="_col2">2つ目のコライダ</param>
	/// <returns>true:当たった</returns>
	const bool IsCollision(const std::weak_ptr<Collider> _col1, const std::weak_ptr<Collider> _col2,VECTOR& hitPos)const;
};