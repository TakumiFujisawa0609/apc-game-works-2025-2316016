#include "../ObjectBase.h"
#include "Collider.h"

Collider::Collider(ObjectBase& parent, TAG tag, Geometry& geometry, std::vector<TAG> notHitTags):parent_(parent),tag_(tag),geometry_(geometry),notHitTags_(notHitTags)
{
	isHit_ = false;
	isDead_ = false;
}

Collider::~Collider(void)
{
	notHitTags_.clear();
}

void Collider::OnHit(const std::weak_ptr<Collider> _collider,VECTOR hitPos)
{
	//この当たり判定が当たった
	isHit_ = true;

	//親に相手のコライダを渡す
	parent_.OnHit(_collider,hitPos);

	//当たり判定が終わった
	isHit_ = false;
}
