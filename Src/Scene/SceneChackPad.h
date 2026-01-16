#pragma once
#include "SceneBase.h"
class SceneChackPad : public SceneBase
{
public:
	static constexpr int BOX_MARGIN = 50;	//”wŒi‚Ì—]”’

	SceneChackPad(void);
	~SceneChackPad(void) override;
	bool Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Load(void) override;
private:

	void DrawBackBox(void);
};