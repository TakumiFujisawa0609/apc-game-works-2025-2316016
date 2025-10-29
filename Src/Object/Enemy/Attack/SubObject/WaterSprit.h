#pragma once
#include <DxLib.h>
#include "SubObjectBase.h"
class WaterSprit :   public SubObjectBase
{
public:

	static constexpr float INIT_SIZE = 50.0f;		//初期サイズ
	static constexpr float SECOND_PER_SIZE = 20.0f;	//1秒あたりのサイズ増加量
	static constexpr float EFFECT_INIT_SIZE = 0.5f;	//エフェクト初期サイズ
	static constexpr float ALIVE_TIME = 15.0f;		//生存時間
	static constexpr float TIME_SCALE = 5.0f;		//時間スケール

	WaterSprit(VECTOR direction, VECTOR startPos , float speed);
	~WaterSprit(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	bool IsEnd(void)const { return isEnd_; }
private:
	VECTOR startPos_;
	VECTOR dir_;
	float rot_;
	int effectHandle_;
	float time_;
	float speed_;
	float size_;
	bool isEnd_;
};

