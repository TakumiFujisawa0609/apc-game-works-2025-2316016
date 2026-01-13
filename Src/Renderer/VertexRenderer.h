#pragma once
#include "BaseRenderer.h"
class VertexMaterial;

class VertexRenderer : public BaseRenderer
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	VertexRenderer(VertexMaterial& material);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~VertexRenderer(void)override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(const int& modelId);

private:

	//モデルのマテリアル
	VertexMaterial& modelMaterial_;

};

