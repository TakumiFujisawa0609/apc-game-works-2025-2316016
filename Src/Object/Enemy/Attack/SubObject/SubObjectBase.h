#pragma once
#include "../../Renderer/Polygon3DRenderer.h"
#include "../../../ObjectModelBase.h"

class AttackBase;

class SubObjectBase : public ObjectModelBase
{
public:
	SubObjectBase(void) = default;
	virtual ~SubObjectBase(void) = default;
	virtual void Init(void) override ;
	virtual void Update(void) override;
	virtual void Draw(void) override;
	float GetDamage(void) const { return damage_; }

protected:
	Polygon3DRenderer::PolygonInfo polInfo_;
	float damage_ = 0.0f;

	virtual void SetPolygonInfo(void);
};

