#pragma once
#include "../../Renderer/Polygon3DRenderer.h"
#include "../../../ObjectModelBase.h"
#include "../Type/AttackBase.h"

class SubObjectBase : public ObjectModelBase
{
public:
	SubObjectBase(AttackBase& parent);
	virtual ~SubObjectBase(void) = default;
	virtual void Init(void) override ;
	virtual void Update(void) override;
	virtual void Draw(void) override;
	float GetDamage(void) const { return damage_; }

protected:
	AttackBase& parent_;
	Polygon3DRenderer::PolygonInfo polInfo_;
	float damage_ = 0.0f;

	virtual void SetPolygonInfo(void);
};

