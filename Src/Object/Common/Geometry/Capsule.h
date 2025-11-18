#pragma once
#include "Geometry.h"
class Capsule :  public Geometry
{
public:

	Capsule( VECTOR& pos1, VECTOR& pos2,float radius);
	~Capsule(void)override;

	void Update(void)override;
	void Draw(void)override;
	GEOMETRY_TYPE GetType(void)override { return GEOMETRY_TYPE::CAPSULE; }


	/// <summary>
	/// 形状当たり判定
	/// </summary>
	/// <param name="_geometry">形状情報</param>
	/// <returns>当たり判定(true:当たった)</returns>
	const bool IsHit(Geometry& _geometry, VECTOR& _hitPos)override;

	/// <summary>
	/// モデルとの当たり判定
	/// </summary>
	/// <param name="_geometry">モデル情報</param>
	/// <returns>当たり判定(true:当たった)</returns>
	//virtual const bool IsHit(Model& _model) = 0;

	/// <summary>
	/// 箱との当たり判定
	/// </summary>
	/// <param name="_geometry">箱情報</param>
	/// <returns>当たり判定(true:当たった)</returns>
	//virtual const bool IsHit(Cube& _cube) = 0;

	/// <summary>
	/// 球との当たり判定
	/// </summary>
	/// <param name="_geometry">球情報</param>
	/// <returns>当たり判定(true:当たった)</returns>
	const bool IsHit(Sphere& _sphere, VECTOR& _hitPos)override;

	/// <summary>
	/// カプセルとの当たり判定
	/// </summary>
	/// <param name="_geometry">カプセル情報</param>
	/// <returns>当たり判定(true:当たった)</returns>
	const bool IsHit(Capsule& _capsule, VECTOR& _hitPos)override;

	/// <summary>
	/// 線分との当たり判定
	/// </summary>
	/// <param name="_geometry">線分情報</param>
	/// <returns>当たり判定(true:当たった)</returns>
	const bool IsHit(Line3D& _line, VECTOR& _hitPos)override;

	/// <summary>
	/// ３角形との当たり判定
	/// </summary>
	/// <param name="_geometry">三角形情報</param>
	/// <returns>当たり判定(true:当たった)</returns>
	const bool IsHit(Triangle3D& _triangle, VECTOR& _hitPos)override;

	/// <summary>
	/// 円柱との当たり判定
	/// </summary>
	/// <param name="_circle">円柱情報</param>
	/// <returns>当たり判定(true:当たった)</returns>
	const bool IsHit(Cylinder& _circle, VECTOR& _hitPos)override;

	/// <summary>
	/// 円周との当たり判定
	/// </summary>
	/// <param name="_circle">円周情報</param>
	/// <returns>当たり判定(true:当たった)</returns>
	const bool IsHit(Circumference& _circle, VECTOR& _hitPos)override;
private:

	static constexpr int DRAW_VERTEX_NUM = 16;

	VECTOR& pos1_;
	VECTOR& pos2_;
	float radius_;
};

