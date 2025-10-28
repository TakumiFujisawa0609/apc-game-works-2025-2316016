#pragma once
#include "../../../ObjectBase.h"

class SubObjectBase : public ObjectBase
{
public:
	SubObjectBase(void) = default;
	virtual ~SubObjectBase(void) = default;
	virtual void Init(void) override = 0;
	virtual void Update(void) override = 0;
	virtual void Draw(void) override = 0;

};

