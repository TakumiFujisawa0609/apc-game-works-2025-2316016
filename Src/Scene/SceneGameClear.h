#pragma once
#include "SceneBase.h"
class SceneGameClear :    public SceneBase
{
public:
	SceneGameClear(void);

	virtual ~SceneGameClear(void) override;

	virtual bool Init(void) override;

	//XVˆ—
	virtual void Update(void) override;

	//•`‰æˆ—
	virtual void Draw(void) override;
private:
};

