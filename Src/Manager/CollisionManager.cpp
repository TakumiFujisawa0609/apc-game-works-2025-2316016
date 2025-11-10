#include "CollisionManager.h"

CollisionManager* CollisionManager::instance_ = nullptr;

void CollisionManager::CreateInstance(void)
{
	//インスタンスの初期生成
	if (instance_ == nullptr)
	{
		instance_ = new CollisionManager();
	}
}

void CollisionManager::AddCollider(const std::shared_ptr<Collider> _collider)
{
	//コライダの追加
	colliders_.push_back(_collider);
}

void CollisionManager::Update(void)
{
	//コライダが一つもないなら処理を飛ばす
	if (colliders_.size() <= 0)return;

	for (int i = 0; i < colliders_.size() - 1; i++)
	{
		//コライダが死亡済みなら
		if (colliders_[i]->IsDead())
		{
			//飛ばす
			continue;
		}

		for (int j = i + 1; j < colliders_.size(); j++)
		{
			//コライダが死亡済みなら
			if (colliders_[i]->IsDead() || colliders_[j]->IsDead())
			{
				//飛ばす
				continue;
			}

			//当たり判定をするか
			if (!JudgeIsCollision(i, j))
			{
				//当たり判定の条件に合わなかったので飛ばす
				continue;
			}

			VECTOR hitPos;
			//当たり判定
			if (IsCollision(colliders_[i], colliders_[j],hitPos))
			{

				//それぞれの当たった処理
				colliders_[i]->OnHit(colliders_[j],hitPos);
				colliders_[j]->OnHit(colliders_[i],hitPos);

				//当たった後の処理
				//if (!colliders_[i]->IsDead())
				//{
				//	colliders_[i]->GetGeometry().HitAfter();
				//}
				//if (!colliders_[j]->IsDead())
				//{
				//	colliders_[j]->GetGeometry().HitAfter();
				//}
			}
		}
	}
	std::erase_if(colliders_, [](std::shared_ptr<Collider> collider) {return collider->IsDead();});
}

CollisionManager::CollisionManager(void)
{

}

CollisionManager::~CollisionManager(void)
{
}

const bool CollisionManager::JudgeIsCollision(const int _col1Num, const int _col2Num) const
{
	Collider::TAG tag1 = colliders_[_col1Num]->GetTag();
	Collider::TAG tag2 = colliders_[_col2Num]->GetTag();
	//違うタグか
	if (tag1 == tag2)
	{
		//同じタグを持っていた
		return false;
	}


	//1人目のタグ
		//2人目の当たり判定しないタグ
	for (auto notColTag2 : colliders_[_col2Num]->GetNotHitTags())
	{
		if (tag1 == notColTag2)
		{
			//1人目のタグが2人目の当たり判定しないタグと同一だった
			return false;
		}
	}

	//2人目のタグ
		//1人目の当たり判定しないタグ
	for (auto notColTag1 : colliders_[_col1Num]->GetNotHitTags())
	{
		if (tag2 == notColTag1)
		{
			//2人目のタグが1人目の当たり判定しないタグと同一だった
			return false;
		}
	}

	//全判定をクリアしたので当たり判定をする
	return true;
}

const bool CollisionManager::IsCollision(const std::weak_ptr<Collider> _col1, const std::weak_ptr<Collider> _col2,VECTOR& hitPos) const
{
	//1つ目の当たり判定形状
	auto& geo1 = _col1.lock()->GetGeometry();

	//2つ目の当たり判定形状
	auto& geo2 = _col2.lock()->GetGeometry();

	//当たり判定
	bool ret = geo1.IsHit(geo2,hitPos);

	//当たり判定
	return ret;
}
