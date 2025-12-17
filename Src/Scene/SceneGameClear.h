#pragma once
#include <memory>
#include "SceneBase.h"

class SkyDome;
class Player;
class Enemy;

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
	std::unique_ptr<SkyDome> skyDome_;
	std::unique_ptr<Player> player_;
	std::unique_ptr<Enemy> enemy_;
};

