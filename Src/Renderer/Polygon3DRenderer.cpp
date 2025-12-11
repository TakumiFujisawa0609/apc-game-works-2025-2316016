#include <algorithm>
#include "Polygon3DRenderer.h"

Polygon3DRenderer::Polygon3DRenderer(Polygon3DMaterial& polygon3DMaterial, PolygonInfo& info) : polygon3DMaterial_(polygon3DMaterial),info_(info)
{
}

Polygon3DRenderer::~Polygon3DRenderer(void)
{
}

void Polygon3DRenderer::Draw(void)
{
	if (isBuckCull_)
	{
		SetUseBackCulling(false);
	}
	drawInfo_ = info_;
	Cull2PolygonInfo();
	// オリジナルシェーダ設定(ON)
	MV1SetUseOrigShader(true);

	//// シェーダ設定(頂点)
	SetReserveVS();

	//// シェーダ設定(ピクセル)
	SetReservePS();

	// テクスチャアドレスタイプの取得
	auto texA = polygon3DMaterial_.GetTextureAddress();
	int texAType = static_cast<int>(texA);

	// テクスチャアドレスタイプを変更
	SetTextureAddressModeUV(texAType, texAType);

	DrawPolygonIndexed3DToShader(drawInfo_.vertex.data(), static_cast<int>(drawInfo_.vertex.size()), drawInfo_.Indices.data(),static_cast<int>(drawInfo_.Indices.size() / POLYGON_VERTEX_NUM));

	// テクスチャアドレスタイプを元に戻す
	SetTextureAddressModeUV(DX_TEXADDRESS_CLAMP, DX_TEXADDRESS_CLAMP);

	// 後始末
	//-----------------------------------------

	// テクスチャ解除
	const auto& textures = polygon3DMaterial_.GetTextures();
	size_t size = textures.size();
	for (int i = 0; i < size; i++)
	{
		SetUseTextureToShader(i, -1);
	}
	//if (size == 0)
	//{
	//	// 前回使用分のテクスチャを引き継がないように
	//	SetUseTextureToShader(0, -1);
	//}
	//else
	//{
	//	for (const auto& pair : textures)
	//	{
	//		SetUseTextureToShader(pair.first, -1);
	//	}
	//}

	// 頂点シェーダ解除
	SetUseVertexShader(-1);

	// ピクセルシェーダ解除
	SetUsePixelShader(-1);

	// オリジナルシェーダ設定(OFF)
	MV1SetUseOrigShader(false);
	//-----------------------------------------
	if (isBuckCull_)
	{
		SetUseBackCulling(true);
	}
}

void Polygon3DRenderer::SetReserveVS(void)
{

	// 定数バッファハンドル
	int constBuf = polygon3DMaterial_.GetConstBufVS();

	FLOAT4* constBufsPtr = (FLOAT4*)GetBufferShaderConstantBuffer(constBuf);
	const auto& constBufs = polygon3DMaterial_.GetConstBufsVS();

	size_t size = constBufs.size();
	for (int i = 0; i < size; i++)
	{
		if (i != 0)
		{
			constBufsPtr++;
		}
		constBufsPtr->x = constBufs[i].x;
		constBufsPtr->y = constBufs[i].y;
		constBufsPtr->z = constBufs[i].z;
		constBufsPtr->w = constBufs[i].w;
	}

	// 頂点シェーダー用の定数バッファを更新して書き込んだ内容を反映する
	UpdateShaderConstantBuffer(constBuf);

	// 頂点シェーダー用の定数バッファを定数バッファレジスタにセット
	SetShaderConstantBuffer(
		constBuf, DX_SHADERTYPE_VERTEX, CONSTANT_BUF_SLOT_BEGIN_VS);

	// 頂点シェーダー設定
	SetUseVertexShader(polygon3DMaterial_.GetShaderVS());

}

void Polygon3DRenderer::SetReservePS(void)
{

	// ピクセルシェーダーにテクスチャを転送
	const auto& textures = polygon3DMaterial_.GetTextures();
	size_t size = textures.size();
	//if (size == 0)
	//{
	//	// 前回使用分のテクスチャを引き継がないように
	//	SetUseTextureToShader(0, -1);
	//}
	//else
	//{
	//	for (const auto& pair : textures)
	//	{
	//		SetUseTextureToShader(pair.first, pair.second);
	//	}
	//}
	for (int i = 0; i < size; i++)
	{
		SetUseTextureToShader(i, textures[i]);
	}
	// 定数バッファハンドル
	int constBuf = polygon3DMaterial_.GetConstBufPS();

	FLOAT4* constBufsPtr = (FLOAT4*)GetBufferShaderConstantBuffer(constBuf);
	const auto& constBufs = polygon3DMaterial_.GetConstBufsPS();

	size = constBufs.size();
	for (int i = 0; i < size; i++)
	{
		if (i != 0)
		{
			constBufsPtr++;
		}
		constBufsPtr->x = constBufs[i].x;
		constBufsPtr->y = constBufs[i].y;
		constBufsPtr->z = constBufs[i].z;
		constBufsPtr->w = constBufs[i].w;
	}

	// ピクセルシェーダー用の定数バッファを更新して書き込んだ内容を反映する
	UpdateShaderConstantBuffer(constBuf);

	// ピクセルシェーダー用の定数バッファを定数バッファレジスタにセット
	SetShaderConstantBuffer(
		constBuf, DX_SHADERTYPE_PIXEL, CONSTANT_BUF_SLOT_BEGIN_PS);

	// ピクセルシェーダー設定
	SetUsePixelShader(polygon3DMaterial_.GetShaderPS());

}

void Polygon3DRenderer::Cull2PolygonInfo(void)
{
	int polygonNum = static_cast<int>(drawInfo_.Indices.size()) / POLYGON_VERTEX_NUM;
	//std::vector<int> eraseIndexes; //消去するindicesの添え字
	std::vector<int> erasePolygon; //消去するindicesの添え字
	for (int i = 0; i < polygonNum; i++)
	{
		VECTOR pos1 = drawInfo_.vertex[drawInfo_.Indices[i * POLYGON_VERTEX_NUM]].pos;
		VECTOR pos2 = drawInfo_.vertex[drawInfo_.Indices[i * POLYGON_VERTEX_NUM + 1]].pos;
		VECTOR pos3 = drawInfo_.vertex[drawInfo_.Indices[i * POLYGON_VERTEX_NUM + 2]].pos;
		VECTOR minV = VGet(
			std::min(pos1.x, std::min(pos2.x, pos3.x)),
			std::min(pos1.y, std::min(pos2.y, pos3.y)),
			std::min(pos1.z, std::min(pos2.z, pos3.z))
		);
		VECTOR maxV = VGet(
			std::max(pos1.x, std::max(pos2.x, pos3.x)),
			std::max(pos1.y, std::max(pos2.y, pos3.y)),
			std::max(pos1.z, std::max(pos2.z, pos3.z))
		);

		if (CheckCameraViewClip_Box(minV, maxV))
		{
			//eraseIndexes.push_back(i * POLYGON_VERTEX_NUM);
			//eraseIndexes.push_back(i * POLYGON_VERTEX_NUM + 1);
			//eraseIndexes.push_back(i * POLYGON_VERTEX_NUM + 2);
			erasePolygon.push_back(i);
		}
	}
	std::vector<unsigned short> newIndices;
	//newIndices.reserve(drawInfo_.Indices.size()); // 必要容量確保

	//for (int i = 0; i < drawInfo_.Indices.size(); i++)
	//{
	//	if (std::find(eraseIndexes.begin(), eraseIndexes.end(), i) != eraseIndexes.end())
	//	{
	//		continue;
	//	}
	//	newIndices.push_back(drawInfo_.Indices[i]);
	//}
	//eraseIndexes.clear();
	for (int i = 0; i < polygonNum; i++)
	{
		if (std::find(erasePolygon.begin(), erasePolygon.end(), i) != erasePolygon.end())
		{
			continue;
		}
		newIndices.push_back(drawInfo_.Indices[i * POLYGON_VERTEX_NUM]);
		newIndices.push_back(drawInfo_.Indices[i * POLYGON_VERTEX_NUM + 1]);
		newIndices.push_back(drawInfo_.Indices[i * POLYGON_VERTEX_NUM + 2]);
	}
	// 差し替え
	drawInfo_.Indices = std::move(newIndices);
	newIndices.clear();
}
