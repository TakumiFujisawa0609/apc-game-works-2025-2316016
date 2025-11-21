#pragma once
#include <DxLib.h>
#include "../Common/Quaternion.h"
#include "KeyConfig.h"
class Transform;

class Camera
{

public:

	// カメラスピード(度)
	static constexpr float SPEED = 1.0f;
	static constexpr float SPEED_PAD = 0.0015f;
	static constexpr float SPEED_MOUSE = 0.05f;

	//カメラズームスピード
	static constexpr float ZOOM_SPEED = 5.0f;

	//カメラのズーム範囲
	static constexpr float ZOOM_RADIUS = 300.0f;
	// カメラクリップ：NEAR
	static constexpr float CAMERA_NEAR = 10.0f;

	// カメラクリップ：NEAR
	static constexpr float CAMERA_FAR = 30000.0f;

	// カメラの初期座標
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };

	// 追従位置からカメラ位置までの相対座標
	static constexpr VECTOR LOCAL_F2C_POS = { 0.0f, 0.0f, -300.0f };
	//static constexpr VECTOR LOCAL_F2C_POS = { 0.0f, -1000.0f, -200.0f };

	//FPSの時の相対座標
	static constexpr VECTOR FPS_LOCAL_F2C_POS = { 0.0f, 130.0f, 20.0f };

	//FPS用
	static constexpr VECTOR FPS_LOCAL_F2T_POS = { 0.0f, 0.0f, 2000.0f };

	//固定上用
	static constexpr VECTOR FIXED_LOCAL_P2T_POS = { 0.0f, -1500.0f, 0.0f };

	//斜め上用
	static constexpr VECTOR FIXED_DIAGONAL_TARGET_POS = { 2000.0f, 00.0f, 3000.0f };

	// 追従位置から注視点までの相対座標
	static constexpr VECTOR LOCAL_F2T_POS = { 0.0f, 0.0f, 100.0f };

	// カメラのX回転上限度角
	static constexpr float LIMIT_X_UP_RAD = 90.0f * (DX_PI_F / 180.0f);
	static constexpr float LIMIT_X_DW_RAD = 15.0f * (DX_PI_F / 180.0f);

	//FPSの上限角
	static constexpr float FPS_LIMIT_X_UP_RAD = -80.0f * (DX_PI_F / 180.0f);
	static constexpr float FPS_LIMIT_X_DW_RAD = 70.0f * (DX_PI_F / 180.0f);

	//カメラの２人同時追従時の最低距離
	static constexpr float TWO_TARGET_MIN_DISTANCE = 100.0f;
	//2人同時追従の基本角度
	static constexpr VECTOR TWO_TARGET_ANGLE = { 0.0f,0.5f,0.2f };
	static constexpr float TWO_TARGET_Y = 0.2f;

	// 2人同時追従の相対座標
	static constexpr VECTOR TWO_TARGET_LOCAL_POS = { 50.0f, 100.0f, -200.0f };

	// カメラモード
	enum class MODE
	{
		NONE,
		FIXED_POINT,
		FOLLOW,
		FOLLOW_ROTATION,	//ついじゅうしながら自動回転操作不能
		//SELF_SHOT,
		//FPS,
		//FREE_CONTROLL,
		FIXED_UP,
		FIXED_DIAGONAL,	//斜め固定
		TWO_TARGET,	//2人同時追従中心
		TWO_TARGET_FOLLOW, //2人同時追従でズーム操作可能1P追従
	};



	Camera(int _playerNum);
	~Camera(void);

	void Init(void);
	void Update(void);
	void SetBeforeDraw(void);
	void Draw(void);

	// カメラ位置
	VECTOR GetPos(void) const;
	// カメラの操作角度
	VECTOR GetAngles(void) const;
	// カメラの注視点
	VECTOR GetTargetPos(void) const;

	// カメラ角度
	Quaternion GetQuaRot(void) const;
	// X回転を抜いたカメラ角度
	Quaternion GetQuaRotOutX(void) const;
	// カメラの前方方向
	VECTOR GetForward(void) const;

	//カメラの設定
	void CameraSetting(void);

	// カメラモードの変更
	void ChangeMode(MODE mode);

	MODE GetMode(void) { return mode_; }

	// 追従対象の設定
	void SetFollow(const Transform* follow);
	void SetFollow(const Transform* follow1, const Transform* follow2);

	void SetPos(VECTOR pos) { pos_ = pos; }
	void SetAngles(VECTOR angles) { angles_ = angles; }
	void SetTargetPos(VECTOR pos) { targetPos_ = pos; }
private:

	// カメラのローカル座標
	VECTOR localPos_;

	// カメラが追従対象とするTransform
	const Transform* followTransform1_;
	const Transform* followTransform2_;

	//PADの番号
	KeyConfig::JOYPAD_NO padNo_;

	// カメラモード
	MODE mode_;

	// カメラの位置
	VECTOR pos_= {0.0f,0.0f,0.0f};

	// カメラ角度(rad)
	VECTOR angles_;

	// X軸回転が無い角度
	Quaternion rotOutX_;

	// カメラ角度
	Quaternion rot_;

	// 注視点
	VECTOR targetPos_;

	// カメラの上方向
	VECTOR cameraUp_;

	// カメラを初期位置に戻す
	void SetDefault(void);

	// 追従対象との位置同期を取る
	void SyncFollow(void);

	//FPS用の位置同期
	//void SyncFollowFPS(void);

	// カメラ操作
	void ProcessRot(void);
	void ProcessZoom(void);
	//マウスでのカメラ操作
	void ProcessRotMause(float* x_m, float* y_m, const float fov_per = 1.0f);
	// モード別更新ステップ
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFollow(void);
	void SetBeforeDrawFollowRotation(void);
	//void SetBeforeDrawSelfShot(void);
	//void SetBeforeDrawFPS(void);
	//void SetBeforeDrawFreeControll(void);
	void SetBeforeDrawFixedUp(void);
	void SetBeforeDrawFixedDiagonal(void);
	void SetBeforeDrawTwoTarget(void);
	void SetBeforeDrawTwoTargetFollow(void);
};

