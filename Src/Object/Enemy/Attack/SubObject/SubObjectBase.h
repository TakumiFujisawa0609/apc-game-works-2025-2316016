#pragma once
#include "../../../ObjectBase.h"

class SubObjectBase : public ObjectBase
{
public:
	SubObjectBase(void) = default;
	virtual ~SubObjectBase(void) = default;
	virtual void Init(void) override ;
	virtual void Update(void) override;
	virtual void Draw(void) override;
	float GetDamage(void) { return damage_; }
protected:
	float damage_;
};

