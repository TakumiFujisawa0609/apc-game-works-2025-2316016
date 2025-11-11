#pragma once
#include <map>
#include <string>
#include "Resource.h"

class ResourceManager
{

public:

	// リソース名
	enum class SRC
	{
		//ボタン
		BUTTON_A,	// Aボタン
		BUTTON_B,	// Bボタン
		BUTTON_X,	// Xボタン
		BUTTON_Y,	// Yボタン
		BUTTON_LB,	// LBボタン
		BUTTON_RB,	// RBボタン
		BUTTON_LT,	// LTボタン
		BUTTON_RT,	// RTボタン
		BUTTON_SELECT,	// SELECTボタン
		BUTTON_START,	// STARTボタン
		BUTTON_UP,	// 十字キー上
		BUTTON_DOWN,	// 十字キー下
		BUTTON_LEFT,	// 十字キー左
		BUTTON_RIGHT,	// 十字キー右

		//操作の文字
		ENTER_STR,	// 決定
		CANCEL_STR,	// キャンセル
		AVOID_STR,	// 回避
		ATTACK_STR,	// 攻撃
		JUMP_STR,	// ジャンプ
		LOCKON_STR,	// ロックオン

		//メニュー用
		BGM_STR,			// BGM
		SE_STR,				// SE
		SCREEN_MODE_STR,	// 画面モード
		FULL_SCREEN_STR,	// フルスクリーン
		WINDOW_SCREEN_STR,	// ウィンドウスクリーン

		//タイトル
		TITLE_IMAGE_1,
		TITLE_IMAGE_2,

		//オブジェクト
		SKY_DOME,	//スカイドーム
		PLAYER,		//プレイヤー
		SHOT,		//プレイヤーショット
		CHICKIN,	//チキン
		SNOW_MAN,	//スノーマン
		DRAGON,		//ドラゴン
		FLOOR,		//ステージ
		SHOCK_WAVE,	//ショックウェーブ
		SHOCK_WAVE1,	//ショックウェーブ
		NOISE,		//ノイズ

		//エフェクト
		HIT_EFFECT,		//ヒットエフェクト
		WATER_SPLIT,	//渦巻
		WATER_SPRIT_MODEL, //ウォータースプリット
		FIRE_MODEL,		//炎のモデル
		THUNDER_MODEL,	//雷のモデル
	};

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static ResourceManager& GetInstance(void);

	// 初期化
	void Init(void);

	// 解放(シーン切替時に一旦解放)
	void Release(void);

	// リソースの完全破棄
	void Destroy(void);

	// リソースのロード
	Resource Load(SRC src);

	// リソースの複製ロード(モデル用)
	int LoadModelDuplicate(SRC src);

private:

	// 静的インスタンス
	static ResourceManager* instance_;

	// リソース管理の対象
	std::map<SRC, Resource> resourcesMap_;

	// 読み込み済みリソース
	std::map<SRC, Resource*> loadedMap_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	ResourceManager(void);

	// デストラクタも同様
	~ResourceManager(void) = default;

	// 内部ロード
	Resource* _Load(SRC src);

};

