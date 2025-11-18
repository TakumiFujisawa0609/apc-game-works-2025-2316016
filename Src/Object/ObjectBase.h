#pragma once
#include <memory>
#include "Common/Collider.h"
#include "Common/Transform.h"
#include "../Renderer/ModelMaterial.h"
#include "../Renderer/ModelRenderer.h"

class ObjectBase
{
public:

	//当たり判定情報
	struct ColParam
	{
		std::unique_ptr<Geometry> geometry_;	//形状情報
		std::shared_ptr<Collider> collider_;	//全体の当たり判定情報
	};

	ObjectBase() = default;
	virtual ~ObjectBase();
	virtual void Init(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	virtual void UIDraw(void);

	virtual void OnHit(const std::weak_ptr<Collider> _hitCol, VECTOR hitPos);
	Transform& GetTransform(void) const{ return *transform_; }
protected:

	//当たり判定関係
	std::vector<ColParam> colParam_;

	//描画関連
	std::shared_ptr<Transform> transform_;
	std::unique_ptr<ModelMaterial> material_;
	std::shared_ptr<ModelRenderer> renderer_;


	/// <summary>
	/// 当たり判定作成(形状情報作成後)
	/// </summary>
	/// <param name="_tag">自身の当たり判定タグ</param>
	/// <param name="_Geometry">自身の形状情報</param>
	/// <param name="_notHitTags">衝突させないタグ</param>
	void MakeCollider(Collider::TAG _tag, std::unique_ptr<Geometry> _geometry, const std::vector<Collider::TAG> _notHitTags = {});
};

