#pragma once
#include <DxLib.h>
#include "../Common/Vector2.h"
#include "../Renderer/Polygon3DMaterial.h"
#include "../Renderer/BaseRenderer.h"
class Polygon3DRenderer : public BaseRenderer
{
public:

	static constexpr int POLYGON_VERTEX_NUM = 3;	//１ポリゴン当たりの頂点数

	//描画情報
	struct PolygonInfo
	{
		std::vector<VERTEX3DSHADER> vertex = {};
		std::vector<unsigned short> Indices = {};
		void clear(void)
		{
			vertex.clear();
			Indices.clear();
		}
	};

	// コンストラクタ
	Polygon3DRenderer(Polygon3DMaterial& polygon3DMaterial,PolygonInfo& info);

	// デストラクタ
	~Polygon3DRenderer(void)override;

	// 描画
	void Draw(void)override;

	//描画情報の設定
	//void SetInfo(const PolygonInfo& info) { info_ = info; }
private:

	//描画に必要な情報
	PolygonInfo& info_;
	//PolygonInfo drawInfo_;
	// ピクセルマテリアル
	Polygon3DMaterial& polygon3DMaterial_;

	// シェーダ設定(頂点)
	void SetReserveVS(void);

	// シェーダ設定(ピクセル)
	void SetReservePS(void);

	//自分のinfo_をカメラ外のポリゴンの場合描画しない
	void Cull2PolygonInfo(void);
};