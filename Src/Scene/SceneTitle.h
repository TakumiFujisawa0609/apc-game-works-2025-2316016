//#pragma once
#include "SceneBase.h"
#include <vector>
#include "../Common/Vector2.h"

class SceneTitle: public SceneBase
{
public:

	//メンバー関数
	//-----------------------------------------------------------
	SceneTitle(void);

	~SceneTitle(void) override;

	//初期化処理(初回の1度のみ実行される)
	bool Init(void)override;

	//更新処理
	void Update(void) override;

	//描画処理
	void Draw(void) override;

private:
};