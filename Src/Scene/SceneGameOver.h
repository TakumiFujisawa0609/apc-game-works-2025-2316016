#pragma once
#include "SceneBase.h"
class SceneGameOver :   public SceneBase
{
public:
	SceneGameOver(void);

	virtual ~SceneGameOver(void) override;

	virtual bool Init(void) override;

	//XVˆ—
	virtual void Update(void) override;

	//•`‰æˆ—
	virtual void Draw(void) override;
private:

};

