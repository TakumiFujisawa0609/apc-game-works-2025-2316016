#pragma once
#include <DxLib.h>

class Capsule;
class Line3D;
class Sphere;
class Triangle3D;

class Geometry
{
public:

	enum class GEOMETRY_TYPE
	{
		CAPSULE,	//カプセル
		SPHERE,		//球
		TRIANGLE3D,	//3D三角形
		LINE3D,		//3D線
	};

	Geometry(void) = default;
	virtual ~Geometry(void) = default;

	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	virtual GEOMETRY_TYPE GetType(void) = 0;

	/// <summary>
	/// 形状当たり判定
	/// </summary>
	/// <param name="_geometry">形状情報</param>
	/// <returns>当たり判定(true:当たった)</returns>
	virtual const bool IsHit(Geometry& _geometry, VECTOR& _hitPos) = 0;

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
	virtual const bool IsHit(Sphere& _sphere ,VECTOR& _hitPos) = 0;

	/// <summary>
	/// カプセルとの当たり判定
	/// </summary>
	/// <param name="_geometry">カプセル情報</param>
	/// <returns>当たり判定(true:当たった)</returns>
	virtual const bool IsHit(Capsule& _capsule, VECTOR& _hitPos) = 0;

	/// <summary>
	/// 線分との当たり判定
	/// </summary>
	/// <param name="_geometry">線分情報</param>
	/// <returns>当たり判定(true:当たった)</returns>
	virtual const bool IsHit(Line3D& _line, VECTOR& _hitPos) = 0;

	/// <summary>
	/// ３角形との当たり判定
	/// </summary>
	/// <param name="_geometry">三角形情報</param>
	/// <returns>当たり判定(true:当たった)</returns>
	virtual const bool IsHit(Triangle3D& _triangle, VECTOR& _hitPos) = 0;
protected:
	static constexpr int DRAW_COLOR = 0xff00ff;
};

