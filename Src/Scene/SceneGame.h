#pragma once
#include"SceneBase.h"
#include"../Common/Vector2.h"

class SceneGame :public SceneBase
{
public:
	//メンバー関数
	//-----------------------------------------------------------
	//初期化処理(初回の1度のみ実行される)
	SceneGame(void) ;

	virtual ~SceneGame(void) override;

	virtual bool Init(void) override;

	//更新処理
	virtual void Update(void) override;

	//描画処理
	virtual void Draw(void) override;

	void DebugDraw(void);
protected:

};