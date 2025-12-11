#pragma once
#include <DxLib.h>
#include "SubObjectBase.h"
#include "../../Renderer/Polygon3DRenderer.h"

class WaterSpritAttack;

class WaterSprit :   public SubObjectBase
{
public:

	static constexpr float INIT_SIZE = 50.0f;		//初期サイズ
	static constexpr float SECOND_PER_SIZE = 20.0f;	//1秒あたりのサイズ増加量
	static constexpr float ALIVE_TIME = 15.0f;		//生存時間
	static constexpr float DAMAGE = 17.5f;	//ダメージ
	static constexpr int VERTEX_NUM = 16;	//描画時の頂点数
	static constexpr float HEIGHT = 750;	//高さ
	static constexpr float TIMES_TOP_SIZE = 3.0f;

	WaterSprit(VECTOR direction, VECTOR startPos , float speed, WaterSpritAttack& parent);
	~WaterSprit(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	bool IsEnd(void)const { return isEnd_; }
	float GetRadius(void)const { return size_; }
private:
	Polygon3DRenderer::PolygonInfo polInfo_;
	WaterSpritAttack& parent_;
	VECTOR startPos_;
	VECTOR dir_;
	float time_;
	float speed_;
	float size_;
	bool isEnd_;

	void SetPolygonInfo(void);
};

