#include "../Manager/CollisionManager.h"
#include "../Renderer/ModelMaterial.h"
#include "../Renderer/ModelRenderer.h"
#include "Common/EffectController.h"
#include "Common/Collider.h"
#include "ObjectBase.h"

ObjectBase::~ObjectBase()
{
	for (auto& colParam : colParam_)
	{
		//所持している全コライダの削除
		colParam.collider_->Kill();
	}

	//当たり判定情報の削除
	colParam_.clear();
}

void ObjectBase::UIDraw(void)
{
}

void ObjectBase::OnHit(const std::weak_ptr<Collider> _hitCol, VECTOR hitPos)
{
}

void ObjectBase::MakeCollider(Collider::TAG _tag, std::unique_ptr<Geometry> _geometry, const std::vector<Collider::TAG> _notHitTags)
{
	//当たり判定情報
	ColParam colParam;

	//形状情報の挿入
	colParam.geometry_ = std::move(_geometry);

	//情報を使ってコライダの作成
	colParam.collider_ = std::make_shared<Collider>(*this, _tag, *colParam.geometry_, _notHitTags);

	//コライダを管理マネージャーに追加
	CollisionManager::GetInstance().AddCollider(colParam.collider_);

	//配列にセット
	colParam_.push_back(std::move(colParam));
}
