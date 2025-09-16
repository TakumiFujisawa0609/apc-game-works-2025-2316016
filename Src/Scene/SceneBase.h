#pragma once
#include <memory>
#include"../Common/Vector2.h"
#include"../Utility/Utility.h"

class SceneBase
{
public:
	SceneBase(void);
	virtual ~SceneBase(void);

	virtual bool Init(void);
	virtual void Update(void);
	virtual void Draw(void);
protected:

private:

	
};