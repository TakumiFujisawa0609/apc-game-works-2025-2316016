#pragma once
#include "SceneBase.h"
#include "../Common/Vector2.h"

class PlayerBase;

class SceneGame :public SceneBase
{
public:
	static constexpr int MAX_PLAYER = 2; //最大プレイヤー数

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

	//プレイヤー
	std::unique_ptr<PlayerBase> players_[MAX_PLAYER];

};