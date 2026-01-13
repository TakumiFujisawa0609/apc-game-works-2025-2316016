#include <math.h>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Utility/Utility.h"
#include "../Object/Common/Transform.h"
#include "../Application.h"
#include "Camera.h"

Camera::Camera(int _playerNum)
{
	angles_ = VECTOR();
	cameraUp_ = VECTOR();
	mode_ = MODE::NONE;
	pos_ = Utility::VECTOR_ZERO;
	targetPos_ = Utility::VECTOR_ZERO;
	padNo_ = static_cast<KeyConfig::JOYPAD_NO>(_playerNum + 1);
	localPos_ = Utility::VECTOR_ZERO;
}

Camera::~Camera(void)
{
}

void Camera::Init(void)
{

	ChangeMode(MODE::FIXED_POINT);

}

void Camera::Update(void)
{
}

void Camera::SetBeforeDraw(void)
{
	VECTOR prePos = pos_;
	// クリップ距離を設定する(SetDrawScreenでリセットされる)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		SetBeforeDrawFixedPoint();
		break;
	case Camera::MODE::FOLLOW:
		SetBeforeDrawFollow();
		break;
	case Camera::MODE::FOLLOW_ROTATION:
		SetBeforeDrawFollowRotation();
		break;
	//case Camera::MODE::SELF_SHOT:
	//	SetBeforeDrawSelfShot();
	//	break;
	//case Camera::MODE::FPS:
	//	SetBeforeDrawFPS();
	//	break;
	//case Camera::MODE::FREE_CONTROLL:
	//	SetBeforeDrawFreeControll();
		break;
	case Camera::MODE::FIXED_UP:
		SetBeforeDrawFixedUp();
		break;
	case Camera::MODE::FIXED_DIAGONAL:
		SetBeforeDrawFixedDiagonal();
		break;
	case Camera::MODE::TWO_TARGET:
		SetBeforeDrawTwoTarget();
		break;
	case Camera::MODE::TWO_TARGET_FOLLOW:
		SetBeforeDrawTwoTargetFollow();
		break;
	}

	pos_ = VAdd(prePos, VScale(VSub(pos_, prePos), POSITION_LERP_POWER));
	//カメラの設定
	CameraSetting();

	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();

}

void Camera::CameraSetting()
{
	// カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		cameraUp_
	);
}

void Camera::Draw(void)
{
}

void Camera::SetFollow( std::shared_ptr<Transform> follow)
{
	followTransform1_ = follow;
}

void Camera::SetFollow( std::shared_ptr<Transform> follow1,  std::shared_ptr<Transform> follow2)
{
	followTransform1_ = follow1;
	followTransform2_ = follow2;
}

VECTOR Camera::GetPos(void) const
{
	return pos_;
}

VECTOR Camera::GetAngles(void) const
{
	return angles_;
}

VECTOR Camera::GetTargetPos(void) const
{
	return targetPos_;
}

Quaternion Camera::GetQuaRot(void) const
{
	return rot_;
}

Quaternion Camera::GetQuaRotOutX(void) const
{
	return rotOutX_;
}

VECTOR Camera::GetForward(void) const
{
	return VNorm(VSub(targetPos_, pos_));
}

void Camera::ChangeMode(MODE mode)
{

	// カメラの初期設定
	//SetDefault();

	// カメラモードの変更
	mode_ = mode;

	// 変更時の初期化処理
	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		break;
	case Camera::MODE::FOLLOW:
	case Camera::MODE::TWO_TARGET_FOLLOW:
		localPos_ = LOCAL_F2C_POS;
		break;
	}

}

void Camera::SetDefault(void)
{

	// カメラの初期設定
	pos_ = DEFAULT_CAMERA_POS;

	// 注視点
	targetPos_ = Utility::VECTOR_ZERO;

	// カメラの上方向
	cameraUp_ = Utility::DIR_U;

	angles_.x = Utility::Deg2RadF(DEFAULT_ANGLE.x);
	angles_.y = Utility::Deg2RadF(DEFAULT_ANGLE.y);
	angles_.z = Utility::Deg2RadF(DEFAULT_ANGLE.z);

	rot_ = Quaternion();

}

void Camera::SyncFollow(void)
{

	// 同期先の位置
	VECTOR pos = followTransform1_->pos;

	// 重力の方向制御に従う
	Quaternion gRot = Quaternion::Euler(VECTOR(0.0, 0.0, 0.0));

	// 正面から設定されたY軸分、回転させる
	rotOutX_ = gRot.Mult(Quaternion::AngleAxis(angles_.y, Utility::AXIS_Y));

	// 正面から設定されたX軸分、回転させる
	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, Utility::AXIS_X));

	VECTOR localPos;

	// 注視点(通常重力でいうところのY値を追従対象と同じにする)
	localPos = rotOutX_.PosAxis(LOCAL_F2T_POS);
	targetPos_ = VAdd(pos, localPos);

	// カメラ位置
	localPos = rot_.PosAxis(localPos_);
	pos_ = VAdd(pos, localPos);

	// カメラの上方向
	cameraUp_ = gRot.GetUp();

}

//void Camera::SyncFollowFPS(void)
//{
//	auto gIns = GravityManager::GetInstance();
//
//	// 同期先の位置
//	VECTOR pos = followTransform1_->pos;
//
//	// 重力の方向制御に従う
//	//Quaternion gRot = gIns.GetTransform().quaRot;
//
//	// 正面から設定されたY軸分、回転させる
//	//rotOutX_ = gRot.Mult(Quaternion::AngleAxis(angles_.y, Utility::AXIS_Y));
//
//	// 正面から設定されたX軸分、回転させる
//	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, Utility::AXIS_X));
//
//	VECTOR localPos;
//
//	// 注視点(通常重力でいうところのY値を追従対象と同じにする)
//	//localPos = rotOutX_.PosAxis(LOCAL_F2T_POS);
//	localPos = rot_.PosAxis(FPS_LOCAL_F2T_POS);
//	targetPos_ = VAdd(pos, localPos);
//
//	// カメラ位置
//	localPos = rotOutX_.PosAxis(FPS_LOCAL_F2C_POS);
//	pos_ = VAdd(pos, localPos);
//
//	// カメラの上方向
//	cameraUp_ = rot_.GetUp();
//}

void Camera::ProcessRot(void)
{
	auto& ins = KeyConfig::GetInstance();
	float rotPow = Utility::Deg2RadF(SPEED);
	if (ins.IsNew(KeyConfig::CONTROL_TYPE::PLAY_CAMERA_MOVE_RIGHT, padNo_)) { angles_.y += rotPow; }
	if (ins.IsNew(KeyConfig::CONTROL_TYPE::PLAY_CAMERA_MOVE_LEFT, padNo_)) { angles_.y -= rotPow; }
	if (ins.IsNew(KeyConfig::CONTROL_TYPE::PLAY_CAMERA_MOVE_UP, padNo_)) { angles_.x += rotPow; }
	if (ins.IsNew(KeyConfig::CONTROL_TYPE::PLAY_CAMERA_MOVE_DOWN, padNo_)) { angles_.x -= rotPow; }


	auto rStick = ins.GetKnockRStickSize(padNo_);
	rotPow = SPEED_PAD;
	angles_.x += Utility::Deg2RadF(rStick.y * rotPow);
	angles_.y += Utility::Deg2RadF(rStick.x * rotPow);
	//マウス
	auto mouseMove = ins.GetMouseMove();
	rotPow = SPEED_MOUSE;
	angles_.x += Utility::Deg2RadF(mouseMove.y * rotPow);
	angles_.y += Utility::Deg2RadF(mouseMove.x * rotPow);
	//画面中央にマウスを固定
	KeyConfig::GetInstance().SetMousePos({ Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y });

	if (angles_.x >= LIMIT_X_UP_RAD)
	{
		angles_.x = LIMIT_X_UP_RAD;
	}
	else if (angles_.x <= LIMIT_X_DW_RAD)
	{
		angles_.x = LIMIT_X_DW_RAD;
	}
}

void Camera::ProcessZoom(void)
{
	auto& ins = KeyConfig::GetInstance();
	auto vec = VNorm(VSub(LOCAL_F2T_POS,LOCAL_F2C_POS));
	if (ins.IsNew(KeyConfig::CONTROL_TYPE::PLAY_CAMERA_ZOOM_IN, padNo_))
	{
		localPos_ = VAdd(localPos_, VScale(vec, ZOOM_SPEED));
	}
	if (ins.IsNew(KeyConfig::CONTROL_TYPE::PLAY_CAMERA_ZOOM_OUT, padNo_))
	{
		localPos_ = VAdd(localPos_, VScale(VScale(vec,-1), ZOOM_SPEED));
	}
	if (Utility::Distance(LOCAL_F2C_POS, localPos_) > ZOOM_RADIUS)
	{
		vec = VNorm(VSub(localPos_,LOCAL_F2C_POS));
		localPos_ = VAdd(LOCAL_F2C_POS, VScale(vec, ZOOM_RADIUS));
	}
}

//void Camera::ProcessRotMause(float* x_m, float* y_m, const float fov_per)
//{
//	int x_t, y_t;
//	GetMousePoint(&x_t, &y_t);
//	*x_m += float(std::clamp(x_t - Application::SCREEN_SIZE_X / 2, -120, 120)) * fov_per / GetFPS();
//	*y_m += float(std::clamp(y_t - Application::SCREEN_SIZE_Y / 2, -120, 120)) * fov_per / GetFPS();
//	SetMousePoint(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2);
//
//	// マウスを表示状態にする
//	SetMouseDispFlag(FALSE);
//
//	if (angles_.x <= FPS_LIMIT_X_UP_RAD)
//	{
//		angles_.x = FPS_LIMIT_X_UP_RAD;
//	}
//	if (angles_.x >= FPS_LIMIT_X_DW_RAD)
//	{
//		angles_.x = FPS_LIMIT_X_DW_RAD;
//	}
//}

void Camera::SetBeforeDrawFixedPoint(void)
{
	// 何もしない
}


void Camera::SetBeforeDrawFollow(void)
{

	// カメラ操作
	ProcessRot();

	ProcessZoom();
	// 追従対象との相対位置を同期
	SyncFollow();
}

void Camera::SetBeforeDrawFollowRotation(void)
{
	const float SPEED_DEG = 1.5f;
	angles_.y += Utility::Deg2RadF(SPEED_DEG);

	auto vec = VNorm(VSub(LOCAL_F2T_POS, LOCAL_F2C_POS));
	localPos_ = VAdd(localPos_, VScale(vec, ZOOM_SPEED));
	if (Utility::Distance(LOCAL_F2C_POS, localPos_) > ZOOM_RADIUS)
	{
		vec = VNorm(VSub(localPos_, LOCAL_F2C_POS));
		localPos_ = VAdd(LOCAL_F2C_POS, VScale(vec, ZOOM_RADIUS));
	}

	// 追従対象との相対位置を同期
	SyncFollow();
}

//void Camera::SetBeforeDrawSelfShot(void)
//{
//	auto gIns = GravityManager::GetInstance();
//
//	// 同期先の位置
//	VECTOR pos = followTransform1_->pos;
//
//
//	// 正面から設定されたX軸分、回転させる
//	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, Utility::AXIS_X));
//
//	VECTOR localPos;
//
//	// 注視点(通常重力でいうところのY値を追従対象と同じにする)
//	localPos = rotOutX_.PosAxis(LOCAL_F2T_POS);
//	targetPos_ = VAdd(pos, localPos);
//
//	// カメラ位置
//	localPos = rot_.PosAxis(LOCAL_F2C_POS);
//	pos_ = VAdd(pos, localPos);
//
//
//}
//
//void Camera::SetBeforeDrawFPS(void)
//{
//	//マウスでのカメラ操作
//	ProcessRotMause(&angles_.y, &angles_.x, 0.2f);
//
//	// 追従対象との相対位置を同期
//	SyncFollowFPS();
//}

//void Camera::SetBeforeDrawFreeControll(void)
//{
//	auto& ins = KeyConfig::GetInstance();
//	float rotPow = Utility::Deg2RadF(SPEED);
//	if (ins.IsNew(KeyConfig::CONTROL_TYPE::EDIT_CAMERA_ROT_RIGHT, padNo_)) { angles_.y += rotPow; }
//	if (ins.IsNew(KeyConfig::CONTROL_TYPE::EDIT_CAMERA_ROT_LEFT, padNo_)) { angles_.y -= rotPow; }
//	if (ins.IsNew(KeyConfig::CONTROL_TYPE::EDIT_CAMERA_ROT_UP, padNo_)) { angles_.x -= rotPow; }
//	if (ins.IsNew(KeyConfig::CONTROL_TYPE::EDIT_CAMERA_ROT_DOWN, padNo_)) { angles_.x += rotPow; }
//
//	if (angles_.x <= FPS_LIMIT_X_UP_RAD)
//	{
//		angles_.x = FPS_LIMIT_X_UP_RAD;
//	}
//	if (angles_.x >= FPS_LIMIT_X_DW_RAD)
//	{
//		angles_.x = FPS_LIMIT_X_DW_RAD;
//	}
//	
//	auto rStick = ins.GetKnockRStickSize(padNo_);
//	rotPow = SPEED_PAD;
//	angles_.x += Utility::Deg2RadF(rStick.y *rotPow);
//	angles_.y += Utility::Deg2RadF(rStick.x *rotPow);
//
//	//if (ins.IsNew(KEY_INPUT_W)) 
//	//{
//	//	pos_ = VAdd(pos_, VScale(Quaternion::Quaternion(angles_).GetForward(), 3.0f));
//	//}
//	//if (ins.IsNew(KEY_INPUT_S))
//	//{
//	//	pos_ = VAdd(pos_, VScale(Quaternion::Quaternion(angles_).GetBack(), moveSpeed));
//	//}
//	static float moveSpeed = 10.0f;
//	static float moveSpeedFB = 30.0f;
//	pos_ = VAdd(pos_, VScale(Quaternion::Quaternion(angles_).GetForward(), ins.IsNew(KeyConfig::CONTROL_TYPE::EDIT_CAMERA_MOVE_FRONT, padNo_) * moveSpeedFB));
//	pos_ = VAdd(pos_, VScale(Quaternion::Quaternion(angles_).GetBack(), ins.IsNew(KeyConfig::CONTROL_TYPE::EDIT_CAMERA_MOVE_BACK, padNo_) * moveSpeedFB));
//	if (ins.IsNew(KeyConfig::CONTROL_TYPE::EDIT_CAMERA_MOVE_LEFT, padNo_))
//	{
//		pos_ = VAdd(pos_, VScale(Quaternion::Quaternion(angles_).GetLeft(), moveSpeed));
//	}
//	if (ins.IsNew(KeyConfig::CONTROL_TYPE::EDIT_CAMERA_MOVE_RIGHT, padNo_))
//	{
//		pos_ =VAdd(pos_, VScale(Quaternion::Quaternion(angles_).GetRight(), moveSpeed));
//	}
//	if (ins.IsNew(KeyConfig::CONTROL_TYPE::EDIT_CAMERA_MOVE_UP, padNo_))
//	{
//		pos_ =VAdd(pos_, VScale(Quaternion::Quaternion(angles_).GetUp(), moveSpeed));
//	}
//	if (ins.IsNew(KeyConfig::CONTROL_TYPE::EDIT_CAMERA_MOVE_DOWN, padNo_))
//	{
//		pos_ =VAdd(pos_, VScale(Quaternion::Quaternion(angles_).GetDown(), moveSpeed));
//	}
//
//	VECTOR localPos;
//	rot_ =(Quaternion::Quaternion(angles_));
//	// 注視点(通常重力でいうところのY値を追従対象と同じにする)
//	localPos = rot_.PosAxis(LOCAL_F2T_POS);
//	targetPos_ = VAdd(pos_, localPos);
//
//
//}

void Camera::SetBeforeDrawFixedUp(void)
{
	targetPos_ = VAdd(pos_, FIXED_LOCAL_P2T_POS);
	rot_ = Quaternion::Quaternion(angles_);
	cameraUp_ = rot_.GetUp();
}

void Camera::SetBeforeDrawFixedDiagonal(void)
{
	targetPos_ = FIXED_DIAGONAL_TARGET_POS;
	rot_ = Quaternion::Quaternion(angles_);
	cameraUp_ = rot_.GetUp();
}

void Camera::SetBeforeDrawTwoTarget(void)
{
	if (followTransform1_ == nullptr || followTransform2_ == nullptr) return;
	VECTOR prePos = pos_;
	// 2つのターゲットの中間地点を注視点にする
	targetPos_ = VScale(VAdd(followTransform1_->pos, followTransform2_->pos), 0.5f);
	// 2つのターゲットの距離に応じてカメラの距離を調整する
	auto dis = VSub(followTransform2_->pos, followTransform1_->pos);
	auto vec = dis;
	vec.y = 0.0f;
	vec = Utility::EqualsVZero(vec) ? vec : VNorm(vec);
	std::swap(vec.x, vec.z);
	vec = VAdd(vec, TWO_TARGET_ANGLE);
	//vec.z = -vec.z;
	vec = VNorm(vec);
	pos_ = VAdd(targetPos_, VScale(VNorm(vec), std::max((Utility::EqualsVZero(dis) ? 1.0f : std::sqrt(dis.x * dis.x + dis.y * dis.y + dis.z * dis.z)), TWO_TARGET_MIN_DISTANCE)));
	pos_ = VAdd(prePos, VScale(VSub(pos_, prePos), POSITION_LERP_POWER));
}
void Camera::SetBeforeDrawTwoTargetFollow(void)
{
	if (followTransform1_ == nullptr || followTransform2_ == nullptr) return;
	targetPos_ = followTransform2_->pos;
	auto pos = followTransform1_->pos;
	auto disY = followTransform2_->pos.y - followTransform1_->pos.y;
	pos.y = (pos.y + followTransform2_->pos.y) / 2;
	auto dis = VSub(targetPos_,pos);
	auto vec = dis;
	vec = VNorm(vec);
	auto back = VScale(vec, TWO_TARGET_LOCAL_POS.z + disY);
	auto up = VGet(0.0f, TWO_TARGET_LOCAL_POS.y, 0.0f);
	VECTOR right = VCross(Utility::DIR_U, vec);
	right = VNorm(right);
	right = VScale(right, TWO_TARGET_LOCAL_POS.x);
	pos_ = VAdd(followTransform1_->pos, VAdd(up, VAdd(back, right)));
	rot_ = Quaternion::LookRotation(VNorm(VSub(targetPos_, pos_)), Utility::DIR_U);
	angles_ = rot_.ToEuler();
	// カメラの上方向
	cameraUp_ =Utility::DIR_U;
}
