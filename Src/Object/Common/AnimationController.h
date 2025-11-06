#pragma once
#include <string>
#include <map>
class SceneManager;

class AnimationController
{
	
public :

	// アニメーションデータ
	struct Animation
	{
		int model = -1;
		int attachNo = -1;
		int animIndex = 0;
		float speed = 0.0f;
		float totalTime = 0.0f;
		float step = 0.0f;
		float blendRate = 0.0f;	// アニメーションブレンド進行度
	};

	static constexpr float DEFAULT_BLEND_ANIM_TIME = 0.5f;

	// コンストラクタ
	AnimationController(int modelId);
	// デストラクタ
	~AnimationController(void);

	// アニメーション追加
	void Add(int type, const std::string& path, float speed);
	void Add(int type, const float speed, int modelId = -1);

	// アニメーション再生
	void Play(int type, bool isLoop = true, 
		float startStep = 0.0f, float endStep = -1.0f, float blendAnimTime = DEFAULT_BLEND_ANIM_TIME, bool isStop = false, bool isForce = false,bool isReverse = false);

	void Update(void);

	// アニメーション終了後に繰り返すループステップ
	void SetEndLoop(float startStep, float endStep, float speed);

	// 再生中のアニメーション
	int GetPlayType(void) const;

	// 再生終了
	bool IsEnd(void) const;

	void DebugDraw();
private :

	// モデルのハンドルID
	int modelId_;

	// 種類別のアニメーションデータ
	std::map<int, Animation> animations_;

	int playType_;
	//Animation playAnim_;

	// アニメーションをループするかしないか
	bool isLoop_;

	// アニメーションを止めたままにする
	bool isStop_;

	// アニメーション終了後に繰り返すループステップ
	float stepEndLoopStart_;
	float stepEndLoopEnd_;
	float endLoopSpeed_;

	// 逆再生
	float switchLoopReverse_;

	//ブレンドアニメーション時間
	float blendAnimTime_;

	// ブレンド
	float blendAnimRate_;

	// 再生中のアニメーションデータマップ
	std::map<int, Animation> playAnimations_;

	// メインの更新処理
	void UpdateMainAnimation();

	// ブレンドアニメーションの更新処理
	void UpdateBlendAnimation();
};

