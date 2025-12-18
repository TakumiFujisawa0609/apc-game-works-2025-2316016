#pragma once
#include "PlayerBase.h"
class ClearPlayer :
    public PlayerBase
{
public:
	ClearPlayer(int playerNum);
	~ClearPlayer(void) override;
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void UIDraw(void) override;
	bool ChangeState(STATE state, bool isAbsolute = false) override; //ó‘Ô•ÏX
protected:
};

