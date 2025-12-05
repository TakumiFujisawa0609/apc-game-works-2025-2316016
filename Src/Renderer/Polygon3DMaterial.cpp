#include "../Application.h"
#include "Polygon3DMaterial.h"

Polygon3DMaterial::Polygon3DMaterial(
	std::string shaderFileNameVS, int constBufFloat4SizeVS,
	std::string shaderFileNamePS, int constBufFloat4SizePS)
{

	// 頂点シェーダのロード
	shaderVS_ = LoadVertexShader(
		(Application::PATH_SHADER + shaderFileNameVS).c_str());

	// 頂点定数バッファの確保サイズ(FLOAT4をいくつ作るか)
	constBufFloat4SizeVS_ = constBufFloat4SizeVS;

	// 頂点シェーダー用の定数バッファを作成
	constBufVS_ = CreateShaderConstantBuffer(sizeof(FLOAT4) * constBufFloat4SizeVS);



	// ピクセルシェーダのロード
	shaderPS_ = LoadPixelShader(
		(Application::PATH_SHADER + shaderFileNamePS).c_str());

	// ピクセル定数バッファの確保サイズ(FLOAT4をいくつ作るか)
	constBufFloat4SizePS_ = constBufFloat4SizePS;

	// ピクセルシェーダー用の定数バッファを作成
	constBufPS_ = CreateShaderConstantBuffer(sizeof(FLOAT4) * constBufFloat4SizePS);



	// テクスチャアドレス
	texAddress_ = TEXADDRESS::CLAMP;

}

void Polygon3DMaterial::AddConstBufVS(const FLOAT4& contBuf)
{

	if (constBufFloat4SizeVS_ > constBufsVS_.size())
	{
		constBufsVS_.emplace_back(contBuf);
	}

}

void Polygon3DMaterial::AddConstBufPS(const FLOAT4& contBuf)
{

	if (constBufFloat4SizePS_ > constBufsPS_.size())
	{
		constBufsPS_.emplace_back(contBuf);
	}

}

void Polygon3DMaterial::SetConstBufVS(int idx, const FLOAT4& contBuf)
{

	if (idx >= constBufsVS_.size())
	{
		return;
	}

	constBufsVS_[idx] = contBuf;

}

void Polygon3DMaterial::SetConstBufPS(int idx, const FLOAT4& contBuf)
{

	if (idx >= constBufsPS_.size())
	{
		return;
	}

	constBufsPS_[idx] = contBuf;

}

void Polygon3DMaterial::AddTextureBuf(int texDiffuse)
{
	if (MAX_TEXTURES > textures_.size())
	{
		textures_.emplace_back(texDiffuse);
	}
}

void Polygon3DMaterial::SetTextureBuf(int idx, int texDiffuse)
{

	if (idx >= textures_.size())
	{
		return;
	}

	textures_[idx] = texDiffuse;

}

const std::vector<FLOAT4>& Polygon3DMaterial::GetConstBufsVS(void) const
{
	return constBufsVS_;
}

const std::vector<FLOAT4>& Polygon3DMaterial::GetConstBufsPS(void) const
{
	return constBufsPS_;
}

const std::vector<int>& Polygon3DMaterial::GetTextures(void) const
{
	return textures_;
}

Polygon3DMaterial::TEXADDRESS Polygon3DMaterial::GetTextureAddress(void) const
{
	return texAddress_;
}

void Polygon3DMaterial::SetTextureAddress(TEXADDRESS texA)
{
	texAddress_ = texA;
}

int Polygon3DMaterial::GetShaderVS(void) const
{
	return shaderVS_;
}

int Polygon3DMaterial::GetShaderPS(void) const
{
	return shaderPS_;
}

int Polygon3DMaterial::GetConstBufVS(void) const
{
	return constBufVS_;
}

int Polygon3DMaterial::GetConstBufPS(void) const
{
	return constBufPS_;
}

Polygon3DMaterial::~Polygon3DMaterial(void)
{
	DeleteShader(shaderVS_);
	DeleteShaderConstantBuffer(constBufVS_);
	DeleteShader(shaderPS_);
	DeleteShaderConstantBuffer(constBufPS_);
}