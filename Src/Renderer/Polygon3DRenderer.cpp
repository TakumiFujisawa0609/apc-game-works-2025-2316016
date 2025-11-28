#include "Polygon3DRenderer.h"

Polygon3DRenderer::Polygon3DRenderer(Polygon3DMaterial& polygon3DMaterial) : polygon3DMaterial_(polygon3DMaterial)
{
}

Polygon3DRenderer::~Polygon3DRenderer(void)
{
}

void Polygon3DRenderer::Draw(void)
{

	// オリジナルシェーダ設定(ON)
	MV1SetUseOrigShader(true);

	// シェーダ設定(頂点)
	SetReserveVS();

	// シェーダ設定(ピクセル)
	SetReservePS();

	// テクスチャアドレスタイプの取得
	auto texA = polygon3DMaterial_.GetTextureAddress();
	int texAType = static_cast<int>(texA);

	// テクスチャアドレスタイプを変更
	SetTextureAddressModeUV(texAType, texAType);

	DrawPolygonIndexed3D(info_.vertex, info_.vertexNum, info_.Indices, info_.polygonNum, DX_NONE_GRAPH, true);

	// テクスチャアドレスタイプを元に戻す
	SetTextureAddressModeUV(DX_TEXADDRESS_CLAMP, DX_TEXADDRESS_CLAMP);

	// 後始末
	//-----------------------------------------

	// テクスチャ解除
	const auto& textures = polygon3DMaterial_.GetTextures();
	size_t size = textures.size();
	if (size == 0)
	{
		// 前回使用分のテクスチャを引き継がないように
		SetUseTextureToShader(0, -1);
	}
	else
	{
		for (const auto& pair : textures)
		{
			SetUseTextureToShader(pair.first, -1);
		}
	}

	// 頂点シェーダ解除
	SetUseVertexShader(-1);

	// ピクセルシェーダ解除
	SetUsePixelShader(-1);

	// オリジナルシェーダ設定(OFF)
	MV1SetUseOrigShader(false);
	//-----------------------------------------

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
	if (size == 0)
	{
		// 前回使用分のテクスチャを引き継がないように
		SetUseTextureToShader(0, -1);
	}
	else
	{
		for (const auto& pair : textures)
		{
			SetUseTextureToShader(pair.first, pair.second);
		}
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