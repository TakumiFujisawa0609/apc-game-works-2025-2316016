#pragma once
#include <DxLib.h>
#include "../Common/Vector2.h"
#include "Polygon2DMaterial.h"
#include "BaseRenderer.h"
class Polygon2DRenderer :  public BaseRenderer
{
public:

	static constexpr int POLYGON_VERTEX_NUM = 3;	//１ポリゴン当たりの頂点数

	//描画情報
	struct PolygonInfo
	{
		std::vector<VERTEX2DSHADER> vertex = {};
		std::vector<unsigned short> Indices = {};
		void clear(void)
		{
			vertex.clear();
			Indices.clear();
		}
	};

	// コンストラクタ
	Polygon2DRenderer(Polygon2DMaterial& polygon2DMaterial, PolygonInfo& info);

	// デストラクタ
	~Polygon2DRenderer(void)override;

	// 描画
	void Draw(void)override;
private:

	//描画に必要な情報
	PolygonInfo& info_;

	// ピクセルマテリアル
	Polygon2DMaterial & polygon2DMaterial_;

	// シェーダ設定(ピクセル)
	void SetReservePS(void);

	//自分のinfo_をカメラ外のポリゴンの場合描画しない
	//void Cull2PolygonInfo(void);

};

