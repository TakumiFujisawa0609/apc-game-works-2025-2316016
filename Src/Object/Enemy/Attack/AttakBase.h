#pragma once
#include <DxLib.h>

class Transform;

class AttakBase
{
public:

	//攻撃優先範囲
	static constexpr float SHORT_RANGE = 100.0f;  //近距離100以内なら優先して発動される
	static constexpr float MIDDLE_RANGE = 300.0f; //中距離300以内なら優先して発動される
	static constexpr float LONG_RANGE = 600.0f;   //遠距離600以内なら優先して発動される

	enum class RANGE	//優先度
	{
		SHORT, //近距離
		MIDDLE,//中距離
		LONG,  //遠距離
	};

	enum class STATE
	{
		NONE,	//実行されていない
		READY, //実行準備
		START,	//実行開始
		UPDATE, //実行中
		FINISH, //実行終了
	};

	AttakBase();
	~AttakBase();
	virtual void Init();
	virtual void Update();
	virtual void Draw();

	void SetTarget(Transform* target) { target_ = target; }

	RANGE GetRange() const { return range_; }

protected:
	RANGE range_; //攻撃範囲
	STATE state_; //状態
	Transform* target_; //相手のTransform
private:
};

