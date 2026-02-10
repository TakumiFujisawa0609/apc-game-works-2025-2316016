#include "Polygon2DRenderer.h"

Polygon2DRenderer::Polygon2DRenderer(Polygon2DMaterial& polygon2DMaterial, PolygonInfo& info) : polygon2DMaterial_(polygon2DMaterial), info_(info)
{
}

Polygon2DRenderer::~Polygon2DRenderer(void)
{
}

void Polygon2DRenderer::Draw(void)
{
	if (isBuckCull_)
	{
		SetUseBackCulling(false);
	}
	// オリジナルシェーダ設定(ON)
	MV1SetUseOrigShader(true);

	//// シェーダ設定(ピクセル)
	SetReservePS();

	// テクスチャアドレスタイプの取得
	auto texA = polygon2DMaterial_.GetTextureAddress();
	int texAType = static_cast<int>(texA);

	// テクスチャアドレスタイプを変更
	SetTextureAddressModeUV(texAType, texAType);

	DrawPolygonIndexed2DToShader(info_.vertex.data(), static_cast<int>(info_.vertex.size()), info_.Indices.data(), static_cast<int>(info_.Indices.size() / POLYGON_VERTEX_NUM));

	// テクスチャアドレスタイプを元に戻す
	SetTextureAddressModeUV(DX_TEXADDRESS_CLAMP, DX_TEXADDRESS_CLAMP);

	// 後始末
	//-----------------------------------------

	// テクスチャ解除
	const auto& textures = polygon2DMaterial_.GetTextures();
	size_t size = textures.size();
	for (int i = 0; i < size; i++)
	{
		SetUseTextureToShader(i, -1);
	}

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

void Polygon2DRenderer::SetReservePS(void)
{
	// ピクセルシェーダーにテクスチャを転送
	const auto& textures = polygon2DMaterial_.GetTextures();
	size_t size = textures.size();
	for (int i = 0; i < size; i++)
	{
		SetUseTextureToShader(i, textures[i]);
	}
	// 定数バッファハンドル
	int constBuf = polygon2DMaterial_.GetConstBuf();

	FLOAT4* constBufsPtr = (FLOAT4*)GetBufferShaderConstantBuffer(constBuf);
	const auto& constBufs = polygon2DMaterial_.GetConstBufs();

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
	SetUsePixelShader(polygon2DMaterial_.GetShader());

}