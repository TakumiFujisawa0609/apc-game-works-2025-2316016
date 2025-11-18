#pragma once
#include <string>
#include <map>

class SceneManager;

class AnimationController
{
public:

	// アニメーションデータ
	struct Animation
	{
		// ------------- 追加ここから -------------
		// 外部アニメとしてロードされたファイルか？
		bool isLoadedFromFile = false;
		// 外部アニメの場合、そのモデルID（内蔵アニメは -1）
		int sourceModel = -1;
		// ------------- 追加ここまで -------------

		int model = -1;          // 再生に使うモデルID（内蔵の場合 modelId_、外部の場合 sourceModel）
		int attachNo = -1;       // AttachAnim の返り値
		int animIndex = 0;       // モデルのアニメ番号
		float speed = 0.0f;
		float totalTime = 0.0f;  // アニメ総時間（秒）
		float step = 0.0f;       // 現在の再生時間（秒）
		float blendRate = 0.0f;  // ブレンド進行度（0..1）
	};

	static constexpr float DEFAULT_BLEND_ANIM_TIME = 0.5f;

	// コンストラクタ
	AnimationController(int modelId);
	// デストラクタ
	~AnimationController(void);

	// アニメーション追加（外部アニメ）
	void Add(int type, const std::string& path, float speed);

	// アニメーション追加（モデル内蔵アニメ）
	void Add(int type, const float speed, int modelId = -1);

	// アニメーション再生
	void Play(int type, bool isLoop = true,
		float startStep = 0.0f, float endStep = -1.0f,
		float blendAnimTime = DEFAULT_BLEND_ANIM_TIME, bool isStop = false,
		bool isForce = false, bool isReverse = false);

	// 更新
	void Update(void);

	// 終了後のループステップ設定
	void SetEndLoop(float startStep, float endStep, float speed);

	// 再生中のアニメーション
	int GetPlayType(void) const;

	// 再生終了
	bool IsEnd(void) const;

	void DebugDraw();

private:

	int modelId_;                       // メインモデル
	std::map<int, Animation> animations_; // 種類別アニメ
	std::map<int, Animation> playAnimations_; // 再生中アニメ

	int playType_;          // 現在のメインアニメ種別
	bool isLoop_;           // ループ再生
	bool isStop_;           // 停止状態

	float stepEndLoopStart_;
	float stepEndLoopEnd_;
	float endLoopSpeed_;
	float switchLoopReverse_;           // 1 or -1

	float blendAnimTime_;               // ブレンド時間（秒）
	float blendAnimRate_;               // 経過ブレンド時間（秒）

	void UpdateMainAnimation();
	void UpdateBlendAnimation();
};
