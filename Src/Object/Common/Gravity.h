#pragma once
#include <DxLib.h>
#include <functional>
#include <map>

class Gravity
{
public:

	enum class STATE
	{
		JUMP,	//空中
		CHANGE,	//方向転換
		NONE,	//地面
	};

	//重力加速度
	static constexpr float DECELERATION = 0.98f;

	Gravity(void);
	~Gravity(void);
	void Init(void);
	void Update(void);
	void Draw(void);

	const VECTOR GetDir(void) const { return dir_; }
	void SetDir(VECTOR dir) { dir_ = dir; }
	void SetInitPower(float power);
	const float GetPower(void) const { return power_; }
	void ChengeState(STATE state);
	STATE GetState(void) { return state_; }
protected:

	//状態ごとの処理
	std::map<STATE, std::function<void(void)>> stateChanges_;
	std::function<void(void)> stateUpdate_;

	//状態遷移処理
	void ChengeStateJump(void);		//空中状態に遷移
	void ChengeStateChange(void);	//方向転換状態に遷移
	void ChengeStateNone(void);		//地面状態に遷移

	//更新処理
	void UpdateStateJump(void);		//空中状態の更新
	void UpdateStateChange(void);	//方向転換状態の更新
	void UpdateStateNone(void);		//地面状態の更新

	//状態
	STATE state_;
	//重力下方向
	VECTOR dir_;
	//重力の強さ
	float power_;
	//状態変更関数セット
	void InitChengeStateFunc(void);
private:

};
