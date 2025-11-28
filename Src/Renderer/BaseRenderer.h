#pragma once
class BaseRenderer
{
public:
	// 頂点シェーダ用オリジナル定数バッファの使用開始スロット
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_VS = 7;
	// ピクセルシェーダ用オリジナル定数バッファの使用開始スロット
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_PS = 4;

	BaseRenderer(void) = default;
	virtual ~BaseRenderer(void) = default;

	virtual void Draw(void);

protected:
};

