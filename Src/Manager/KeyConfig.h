#pragma once
#include <vector>
#include <map>
#include <memory>
#include "../Common/Vector2.h"

class InputManager;

class KeyConfig
{
public:

	// ゲームコントローラーの認識番号
	// DxLib定数、DX_INPUT_PAD1等に対応
	enum class JOYPAD_NO
	{
		KEY_PAD1,			// キー入力とパッド１入力
		PAD1,				// パッド１入力
		PAD2,				// パッド２入力
		PAD3,				// パッド３入力
		PAD4,				// パッド４入力
		INPUT_KEY = 4096	// キー入力
	};

	enum class TYPE
	{
		KEYBORD_MOUSE,
		PAD,
		ALL,
	};


	// ゲームコントローラーボタン
	enum class JOYPAD_BTN
	{
		RIGHTBUTTON_LEFT = 0,	// X
		RIGHTBUTTON_RIGHT,		// B
		RIGHTBUTTON_TOP,		// Y
		RIGHTBUTTON_DOWN,		// A
		R_TRIGGER,				// R_TRIGGER
		L_TRIGGER,				// L_TRIGGER
		R_BUTTON,				// R_BUTTON
		L_BUTTON,				// L_BUTTON
		START_BUTTON,			// START_BUTTON
		SELECT_BUTTON,			// SELECT_BUTTON
		LEFTBUTTON_TOP,			// 上
		LEFTBUTTON_DOWN,		// 下
		LEFTBUTTON_LEFT,		// 左
		LEFTBUTTON_RIGHT,		// 右
		LEFT_STICK,				// 左スティック押し込み
		RIGHT_STICK,			// 右スティック押し込み
		MAX
	};

	enum class JOYPAD_STICK
	{
		L_STICK_UP,		//左スティック上
		L_STICK_DOWN,	//左スティック下
		L_STICK_LEFT,	//左スティック左
		L_STICK_RIGHT,	//左スティック右
		R_STICK_UP,		//右スティック上
		R_STICK_DOWN,	//右スティック下
		R_STICK_LEFT,	//右スティック左
		R_STICK_RIGHT,	//右スティック右
		MAX
	};

	enum class MOUSE
	{
		CLICK_RIGHT,		//右クリック
		CLICK_LEFT,			//左クリック
		MOVE_LEFT,			//左移動
		MOVE_RIGHT,			//右移動
		MOVE_UP,			//上移動
		MOVE_DOWN,			//下移動
		WHEEL_FRONT,		//ホイール前(奥)回転
		WHEEL_BACK,			//ホイール後ろ(手前)回転
		MAX
	};

	enum class CONTROL_TYPE //操作の種類	
	{
		ENTER,					//決定

		CHENGE_CAMERA_MODE,	//カメラモード変更

		PHASE_CHENGE,			//フェーズ変更
		PHASE_CHENGE_CHECK,		//フェーズ変更

		PLAY_CAMERA_MOVE_UP,	//プレイのカメラ移動上
		PLAY_CAMERA_MOVE_DOWN,	//プレイのカメラ移動下
		PLAY_CAMERA_MOVE_RIGHT,	//プレイのカメラ移動右
		PLAY_CAMERA_MOVE_LEFT,	//プレイのカメラ移動左
		PLAY_CAMERA_ZOOM_IN,	//プレイのカメラズームイン
		PLAY_CAMERA_ZOOM_OUT,	//プレイのカメラズームアウト

		PLAYER_MOVE_UP,		//プレイヤー移動上
		PLAYER_MOVE_DOWN,		//プレイヤー移動下
		PLAYER_MOVE_RIGHT,		//プレイヤー移動右
		PLAYER_MOVE_LEFT,		//プレイヤー移動左
		PLAYER_AVOID,			//プレイヤー回避
		PLAYER_ATTACK,			//プレイヤー攻撃
		PLAYER_JUMP,			//プレイヤージャンプ
		MAX,
	};

	// インスタンスを明示的に生成
	static void CreateInstance(void);

	// インスタンスの取得
	static KeyConfig& GetInstance(void);

	void Init(void);
	void Update(void);

	//操作の種類別にキーの状態を取得
	bool IsNew(CONTROL_TYPE cType, KeyConfig::JOYPAD_NO no ,TYPE type = TYPE::ALL);
	bool IsTrgDown(CONTROL_TYPE cType, KeyConfig::JOYPAD_NO no,TYPE type = TYPE::ALL);
	bool IsTrgUp(CONTROL_TYPE cType, KeyConfig::JOYPAD_NO no, TYPE type = TYPE::ALL);

	//操作の種類別にキーを追加	(初期追加の固定)
	void AddFixed(CONTROL_TYPE type, int key);
	void AddFixed(CONTROL_TYPE type, JOYPAD_BTN key);
	void AddFixed(CONTROL_TYPE type, JOYPAD_STICK key);
	void AddFixed(CONTROL_TYPE type, MOUSE key);

	// マウス座標の取得
	Vector2 GetMousePos(void) const;
	//マウスの移動量を取得
	Vector2 GetMouseMove(void) const;

	//マウスの座標を設定
	void SetMousePosScreen(void);

	void SetMousePos(const Vector2& pos);
	float GetLStickDeg(KeyConfig::JOYPAD_NO no) const;

	float GetRStickDeg(KeyConfig::JOYPAD_NO no) const;
	//上を0.0度として角度を渡す
	Vector2 GetKnockLStickSize(KeyConfig::JOYPAD_NO no) const;
	Vector2 GetKnockRStickSize(KeyConfig::JOYPAD_NO no) const;
	//指定の方向に倒れた度合い0から1000
	int PadStickOverSize(KeyConfig::JOYPAD_NO no, KeyConfig::JOYPAD_STICK stick)const;
	// リソースの破棄
	void Destroy(void);
	/// <summary>
	/// パッドを振動する
	/// </summary>
	/// <param name="_no">振動させるPAD番号</param>
	/// <param name="_time">ミリ秒　,-1で無限に続けることができる(STOP 必須)</param>
	/// <param name="_pow">1～1000</param>
	void PadVibration(KeyConfig::JOYPAD_NO _no, int _time, int _pow);
	/// <summary>
	/// 振動を止める
	/// </summary>
	/// <param name="_no"></param>
	void StopPadVibration(KeyConfig::JOYPAD_NO _no);
private:
	std::unique_ptr<InputManager> inputManager_;	//入力管理クラスのインスタンス

	std::map<CONTROL_TYPE, std::vector<int>>keyInput_;								//操作の種類とキーの種類でキーボードの状態を格納
	std::map<CONTROL_TYPE, std::vector<JOYPAD_BTN>>conInput_;			//操作の種類とボタンの種類でコントローラーの状態を格納
	std::map<CONTROL_TYPE, std::vector<JOYPAD_STICK>>stickInput_;		//操作の種類とスティックの種類でコントローラーの状態を格納
	std::map < CONTROL_TYPE, std::vector<MOUSE>>mouseInput_;			//操作の種類とマウスの種類でマウスの状態を格納

	// シングルトン用インスタンス
	static KeyConfig* instance_;

	KeyConfig(void);
	KeyConfig(const KeyConfig& manager);
	~KeyConfig(void) = default;
};

