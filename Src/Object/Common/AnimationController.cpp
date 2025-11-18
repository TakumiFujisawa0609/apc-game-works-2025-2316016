#include "AnimationController.h"
#include <DxLib.h>
#include "../../Manager/SceneManager.h"

// ------------------------------------------------------
// コンストラクタ
// ------------------------------------------------------
AnimationController::AnimationController(int modelId)
	: modelId_(modelId)
	, playType_(-1)
	, isLoop_(false)
	, isStop_(false)
	, stepEndLoopStart_(-1.0f)
	, stepEndLoopEnd_(-1.0f)
	, endLoopSpeed_(0.0f)
	, switchLoopReverse_(1.0f)
	, blendAnimTime_(DEFAULT_BLEND_ANIM_TIME)
	, blendAnimRate_(0.0f)
{
}

// ------------------------------------------------------
// デストラクタ（外部アニメだけ破棄）
// ------------------------------------------------------
AnimationController::~AnimationController(void)
{
	for (auto& kv : animations_)
	{
		Animation& anim = kv.second;
		// 外部アニメとしてロードしたモデルだけ削除する
		if (anim.isLoadedFromFile && anim.sourceModel != -1)
		{
			MV1DeleteModel(anim.sourceModel);
			anim.sourceModel = -1;
		}
	}
}

// ------------------------------------------------------
// 外部アニメ追加（mv1 から読み込む）
// ------------------------------------------------------
void AnimationController::Add(int type, const std::string& path, float speed)
{
	Animation anim;

	anim.isLoadedFromFile = true;
	anim.sourceModel = MV1LoadModel(path.c_str());
	anim.model = anim.sourceModel;        // Attach 時に参照するモデル
	anim.animIndex = 0;                   // 外部アニメは 0 番固定（慣習）
	anim.speed = speed;

	// attach は Play 時に行うため総時間は 0 としておく（Play 内で取得）
	anim.totalTime = 0.0f;
	anim.step = 0.0f;
	anim.attachNo = -1;
	anim.blendRate = 0.0f;

	animations_[type] = anim;
}

// ------------------------------------------------------
// 内蔵アニメ追加（modelId から直接使う）
// ------------------------------------------------------
void AnimationController::Add(int type, const float speed, int modelId)
{
	Animation anim;

	anim.isLoadedFromFile = false;
	anim.sourceModel = -1;

	if (modelId != -1)
	{
		anim.model = modelId; // 指定モデル
	}
	else
	{
		anim.model = modelId_; // 自分のメインモデル
	}

	anim.animIndex = type; // 内蔵アニメは type 番号を使う
	anim.speed = speed;

	// attach は Play 時に行うため総時間は 0 としておく
	anim.totalTime = 0.0f;
	anim.step = 0.0f;
	anim.attachNo = -1;
	anim.blendRate = 0.0f;

	animations_[type] = anim;
}

// ------------------------------------------------------
// 再生開始（同一 type の連続再生を適切に扱う）
// ------------------------------------------------------
void AnimationController::Play(
	int type, bool isLoop,
	float startStep, float endStep, float blendAnimTime,
	bool isStop, bool isForce, bool isReverse)
{
	// 即時切替（負値は「即時切替」のシグナル）
	const bool immediateSwitchRequested = (blendAnimTime < 0.0f);

	// 再生対象が存在しないなら何もしない
	auto itTemplate = animations_.find(type);
	if (itTemplate == animations_.end())
	{
		return;
	}

	// 同一 type の連続 Play をどう扱うか判定する
	bool sameType = (type == playType_);
	if (sameType && !isForce && !immediateSwitchRequested)
	{
		// 再生中のアニメ情報があれば終了判定を見る
		auto itCurrent = playAnimations_.find(playType_);
		if (itCurrent != playAnimations_.end())
		{
			const Animation& cur = itCurrent->second;
			// 再生が終了していない（まだ再生中）なら早期リターン
			// ※ cur.totalTime が 0 の場合は未取得なので再生中と判断する
			if (cur.totalTime > 0.0f && cur.step < cur.totalTime)
			{
				return;
			}
			// 終了しているならリスタートを許可する（以下で上書き処理）
		}
		else
		{
			// playAnimations_ に現在アニメが無い場合は通常処理を継続
		}
	}

	// 各種フラグ反映
	playType_ = type;
	isLoop_ = isLoop;
	isStop_ = isStop;
	switchLoopReverse_ = isReverse ? -1.0f : 1.0f;

	blendAnimTime_ = blendAnimTime;
	blendAnimRate_ = 0.0f;

	// テンプレートからコピーして準備
	Animation anim = itTemplate->second;
	anim.step = startStep;

	// endStep 指定があれば上書き
	if (endStep > 0.0f)
	{
		anim.totalTime = endStep;
	}

	// Attach（既存 attach があれば安全にデタッチして置き換える）
	auto itPlaying = playAnimations_.find(type);
	if (itPlaying != playAnimations_.end())
	{
		// 既に同じ type が登録されている（終了済みでリスタートする場合等）
		if (itPlaying->second.attachNo != -1)
		{
			MV1DetachAnim(modelId_, itPlaying->second.attachNo);
		}
		playAnimations_.erase(itPlaying);
	}

	// アニメをモデルにアタッチする
	// anim.animIndex は Add() で正しく設定済み
	anim.attachNo = MV1AttachAnim(modelId_, anim.animIndex, anim.model);

	// アタッチ後に総時間を取得（Add() 時には取得していないため）
	if (anim.totalTime <= 0.0f)
	{
		anim.totalTime = MV1GetAttachAnimTotalTime(modelId_, anim.attachNo);
	}

	// ブレンド初期値
	if (playAnimations_.empty())
	{
		// 最初のアニメなら完全に表示
		anim.blendRate = 1.0f;
		MV1SetAttachAnimBlendRate(modelId_, anim.attachNo, anim.blendRate);
	}
	else
	{
		// 既に別アニメがある場合は通常 0 から開始だが、
		// 即時切替要求がある場合は 1 にして他を即座に消す
		if (immediateSwitchRequested)
		{
			anim.blendRate = 1.0f;
			MV1SetAttachAnimBlendRate(modelId_, anim.attachNo, anim.blendRate);

			// 他のアニメは即時デタッチ
			for (auto it = playAnimations_.begin(); it != playAnimations_.end(); )
			{
				MV1DetachAnim(modelId_, it->second.attachNo);
				it = playAnimations_.erase(it);
			}
		}
		else
		{
			anim.blendRate = 0.0f;
			MV1SetAttachAnimBlendRate(modelId_, anim.attachNo, anim.blendRate);
			// 旧アニメは残してブレンドさせる
		}
	}

	// 登録（同じ type が残っていれば上書き）
	playAnimations_[type] = anim;
}

// ------------------------------------------------------
// 毎フレーム更新
// ------------------------------------------------------
void AnimationController::Update(void)
{
	if (isStop_) return;

	// playAnimations_ が空なら何もしない
	if (playAnimations_.empty()) return;

	UpdateMainAnimation();
	UpdateBlendAnimation();
}

// ------------------------------------------------------
// EndLoop設定
// ------------------------------------------------------
void AnimationController::SetEndLoop(float startStep, float endStep, float speed)
{
	stepEndLoopStart_ = startStep;
	stepEndLoopEnd_ = endStep;
	endLoopSpeed_ = speed;
}

// ------------------------------------------------------
// 現在再生中アニメタイプ
// ------------------------------------------------------
int AnimationController::GetPlayType(void) const
{
	return playType_;
}

// ------------------------------------------------------
// 再生終了判定
// ------------------------------------------------------
bool AnimationController::IsEnd(void) const
{
	auto it = playAnimations_.find(playType_);
	if (it == playAnimations_.end())
	{
		return false;
	}

	const Animation& playAnim = it->second;

	// ループ設定があるなら「終了とは見なさない」
	if (isLoop_) return false;

	if (switchLoopReverse_ > 0.0f)
	{
		return (playAnim.step >= playAnim.totalTime);
	}
	else
	{
		return (playAnim.step <= 0.0f);
	}
}

// ------------------------------------------------------
// デバッグ描画
// ------------------------------------------------------
void AnimationController::DebugDraw()
{
	int y = 20;
	float total = 0.0f;

	for (auto& kv : playAnimations_)
	{
		int type = kv.first;
		const Animation& a = kv.second;
		DrawFormatString(0, y, 0xff0000, "type:%d attach:%d blend:%f step:%f",
			type, a.attachNo, a.blendRate, a.step);
		y += 20;
		total += a.blendRate;
	}

	DrawFormatString(0, 0, 0xff0000, "blendTotal:%f", total);

	if (playAnimations_.count(playType_))
	{
		DrawFormatString(0, 100, 0xff0000, "MainAttach:%d",
			playAnimations_.at(playType_).attachNo);
	}
}

// ------------------------------------------------------
// メインアニメ更新
// ------------------------------------------------------
void AnimationController::UpdateMainAnimation()
{
	// 再生中の main アニメが存在しなければ終了
	auto itMain = playAnimations_.find(playType_);
	if (itMain == playAnimations_.end()) return;

	Animation& anim = itMain->second;

	float dt = SceneManager::GetInstance().GetDeltaTime();

	// ストップ中でなければ時間を進める（逆再生は switchLoopReverse_ により符号が入る）
	if (!isStop_)
	{
		anim.step += dt * anim.speed * switchLoopReverse_;
	}

	// 終了判定（forward: step > totalTime, reverse: step < 0）
	bool isEnd = false;
	if (switchLoopReverse_ > 0.0f)
	{
		if (anim.step > anim.totalTime)
			isEnd = true;
	}
	else
	{
		if (anim.step < 0.0f)
			isEnd = true;
	}

	if (isEnd)
	{
		if (isLoop_)
		{
			if (stepEndLoopStart_ >= 0.0f)
			{
				// 指定区間ループ（往復）
				switchLoopReverse_ *= -1.0f;

				if (switchLoopReverse_ > 0.0f)
				{
					anim.step = stepEndLoopStart_;
					anim.totalTime = stepEndLoopEnd_;
				}
				else
				{
					anim.step = stepEndLoopEnd_;
					anim.totalTime = stepEndLoopStart_;
				}

				anim.speed = endLoopSpeed_;
			}
			else
			{
				// 通常ループ（時間を折り返す）
				if (switchLoopReverse_ > 0.0f)
					anim.step = 0.0f;
				else
					anim.step = anim.totalTime;
			}
		}
		else
		{
			// 非ループは終端で固定
			if (switchLoopReverse_ > 0.0f)
				anim.step = anim.totalTime;
			else
				anim.step = 0.0f;
		}
	}

	// アニメ時間（秒）をセット
	MV1SetAttachAnimTime(modelId_, anim.attachNo, anim.step);
}

// ------------------------------------------------------
// ブレンド更新
// ------------------------------------------------------
void AnimationController::UpdateBlendAnimation()
{
	// ブレンド対象が 1 以下なら何もしない
	if (static_cast<int>(playAnimations_.size()) <= 1)
	{
		return;
	}

	float dt = SceneManager::GetInstance().GetDeltaTime();

	// 即時切替（blendAnimTime_ <= 0） の扱い
	if (blendAnimTime_ <= 0.0f)
	{
		// 新アニメを 1.0 にし、他は即座に削除する
		for (auto it = playAnimations_.begin(); it != playAnimations_.end(); )
		{
			if (it->first == playType_)
			{
				it->second.blendRate = 1.0f;
				MV1SetAttachAnimBlendRate(modelId_, it->second.attachNo, 1.0f);
				++it;
			}
			else
			{
				MV1DetachAnim(modelId_, it->second.attachNo);
				it = playAnimations_.erase(it);
			}
		}
		// 経過は設定しておく（安定化のため）
		blendAnimRate_ = blendAnimTime_ <= 0.0f ? 0.0f : blendAnimTime_;
		return;
	}

	// 通常の線形ブレンド（blendAnimRate_ は秒単位の経過）
	blendAnimRate_ += dt;
	if (blendAnimRate_ > blendAnimTime_) blendAnimRate_ = blendAnimTime_;

	float t = blendAnimRate_ / blendAnimTime_;
	if (t < 0.0f) t = 0.0f;
	if (t > 1.0f) t = 1.0f;

	for (auto it = playAnimations_.begin(); it != playAnimations_.end(); )
	{
		if (it->first == playType_)
		{
			it->second.blendRate = t;
			MV1SetAttachAnimBlendRate(modelId_, it->second.attachNo, it->second.blendRate);
			++it;
		}
		else
		{
			it->second.blendRate = 1.0f - t;
			if (it->second.blendRate <= 0.001f)
			{
				MV1DetachAnim(modelId_, it->second.attachNo);
				it = playAnimations_.erase(it);
			}
			else
			{
				MV1SetAttachAnimBlendRate(modelId_, it->second.attachNo, it->second.blendRate);
				++it;
			}
		}
	}
}
