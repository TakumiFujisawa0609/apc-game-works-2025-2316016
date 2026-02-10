#pragma once
#include <string>
#include <vector>
#include <map>
#include <DxLib.h>

class Polygon2DMaterial
{
public:
	// 設定できる最大のテクスチャの数
	static constexpr int MAX_TEXTURES = 4;

	// テクスチャアドレスタイプ
	enum class TEXADDRESS
	{
		NONE = 0,
		WRAP,
		MIRROR,
		CLAMP,
		BORDER,
		MAX
	};

	// コンストラクタ
	Polygon2DMaterial(
		std::string shaderFileName, int constBufFloat4Size
	);

	// デストラクタ
	~Polygon2DMaterial(void);

	// 定数バッファを追加
	void AddConstBuf(const FLOAT4& contBuf);

	// 定数バッファを更新
	void SetConstBuf(int idx, const FLOAT4& contBuf);

	// テクスチャバッファを追加
	void AddTextureBuf(int texDiffuse);
	// テクスチャを更新
	void SetTextureBuf(int idx, int texDiffuse);

	// シェーダ
	int GetShader(void) const;

	// 定数バッファハンドル
	int GetConstBuf(void) const;

	// 定数バッファ
	const std::vector<FLOAT4>& GetConstBufs(void) const;

	// 画像
	//const std::map<int, int>& GetTextures(void) const;
	// 画像
	const std::vector<int>& GetTextures(void) const;

	// テクスチャアドレスを取得
	TEXADDRESS GetTextureAddress(void) const;

	// テクスチャアドレスを設定
	void SetTextureAddress(TEXADDRESS texA);

private:

	// ピクセルシェーダ
	int shader_;

	// ピクセル定数バッファの確保サイズ(FLOAT4をいくつ作るか)
	int constBufFloat4Size_;

	// ピクセル定数バッファハンドル
	int constBuf_;


	// テクスチャアドレス
	TEXADDRESS texAddress_;

	// ピクセル定数バッファ
	std::vector<FLOAT4> constBufs_;

	// 画像
	std::vector<int> textures_;
};

