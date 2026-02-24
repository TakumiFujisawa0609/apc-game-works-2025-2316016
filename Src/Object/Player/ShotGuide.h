#pragma once
#include <DxLib.h>
#include "../../Object/ObjectBase.h"
class Gravity;

class ShotGuide : public ObjectBase
{
public:
	static constexpr int HIT_COLOR = 0xffff00; //当たったときの色
	static constexpr int NON_HIT_COLOR = 0x00ffff; //当たらないときの色
	static constexpr int MAX_GUIDE_NUM = 70; //ガイドの最大数

	ShotGuide(VECTOR& handPos);
	~ShotGuide();
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	void OnHit(const std::weak_ptr<Collider> _hitCol, VECTOR hitPos)override;
	void SetTargetPos(VECTOR tPos) { targetPos_ = tPos; }

private:
	VECTOR targetPos_;	//ターゲット座標
	VECTOR& handPos_;	//手の座標
	//重力
	std::unique_ptr<Gravity> gravity_;
	bool isHit_;	//当たるか
	std::vector<VECTOR> guidePoints_;	//ガイドの点
};

