#pragma once
#include <DxLib.h>
#include "../Common/Vector2.h"
#include "../Renderer/ModelMaterial.h"
#include "../Renderer/BaseRenderer.h"

class ModelRenderer : public BaseRenderer
{

public:

	// コンストラクタ
	ModelRenderer(int modelId_, ModelMaterial& modelMaterial);

	// デストラクタ
	~ModelRenderer(void)override;

	// 描画
	void Draw(void)override;

	//メッシュごとに描画
	void DrawMeshes(void);

private:

	// モデルのハンドルID
	int modelId_;

	// モデルマテリアル
	ModelMaterial& modelMaterial_;

	// シェーダ設定(頂点)
	void SetReserveVS(void);

	// シェーダ設定(ピクセル)
	void SetReservePS(void);

};
