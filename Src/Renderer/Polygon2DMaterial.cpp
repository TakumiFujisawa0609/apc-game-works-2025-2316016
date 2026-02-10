#include "../Application.h"
#include "Polygon2DMaterial.h"

Polygon2DMaterial::Polygon2DMaterial(std::string shaderFileName, int constBufFloat4Size)
{
	// ピクセルシェーダのロード
	shader_ = LoadPixelShader(
		(Application::PATH_SHADER + shaderFileName).c_str());

	// ピクセル定数バッファの確保サイズ(FLOAT4をいくつ作るか)
	constBufFloat4Size_ = constBufFloat4Size;

	// ピクセルシェーダー用の定数バッファを作成
	constBuf_ = CreateShaderConstantBuffer(sizeof(FLOAT4) * constBufFloat4Size);



	// テクスチャアドレス
	texAddress_ = TEXADDRESS::CLAMP;
}

Polygon2DMaterial::~Polygon2DMaterial(void)
{
}
// 定数バッファを追加
void Polygon2DMaterial::AddConstBuf(const FLOAT4& contBuf)
{
	if (constBufFloat4Size_ > constBufs_.size())
	{
		constBufs_.emplace_back(contBuf);
	}
}

// 定数バッファを更新
void Polygon2DMaterial::SetConstBuf(int idx, const FLOAT4& contBuf)
{
	if (idx >= constBufs_.size())
	{
		return;
	}

	constBufs_[idx] = contBuf;

}

// テクスチャバッファを追加
void Polygon2DMaterial::AddTextureBuf(int texDiffuse)
{
	if (MAX_TEXTURES > textures_.size())
	{
		textures_.emplace_back(texDiffuse);
	}
}
// テクスチャを更新
void Polygon2DMaterial::SetTextureBuf(int idx, int texDiffuse)
{
	if (idx >= textures_.size())
	{
		return;
	}

	textures_[idx] = texDiffuse;
}

// シェーダ
int Polygon2DMaterial::GetShader(void) const
{
	return shader_;
}

// 定数バッファハンドル
int Polygon2DMaterial::GetConstBuf(void) const
{
	return constBuf_;
}

// 定数バッファ
const std::vector<FLOAT4>& Polygon2DMaterial::GetConstBufs(void) const
{
	return constBufs_;
}

// 画像
const std::vector<int>& Polygon2DMaterial::GetTextures(void) const
{
	return textures_;
}

// テクスチャアドレスを取得
Polygon2DMaterial::TEXADDRESS Polygon2DMaterial::GetTextureAddress(void) const
{
	return texAddress_;
}

// テクスチャアドレスを設定
void Polygon2DMaterial::SetTextureAddress(TEXADDRESS texA)
{
	texAddress_ = texA;
}
