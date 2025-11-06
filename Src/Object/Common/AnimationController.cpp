#include <DxLib.h>
#include "../../Manager/SceneManager.h"
#include "AnimationController.h"

AnimationController::AnimationController(int modelId)
{
	modelId_ = modelId;

	playType_ = -1;
	isLoop_ = false;

	isStop_ = false;
	switchLoopReverse_ = 0.0f;
	endLoopSpeed_ = 0.0f;
	stepEndLoopStart_ = 0.0f;
	stepEndLoopEnd_ = 0.0f;
}

AnimationController::~AnimationController(void)
{
	for (const auto& anim : animations_)
	{
		MV1DeleteModel(anim.second.model);
	}
}

void AnimationController::Add(int type, const std::string& path, float speed)
{

	Animation anim;

	anim.model = MV1LoadModel(path.c_str());
	anim.animIndex = type;
	anim.speed = speed;

	if (animations_.count(type) == 0)
	{
		// 入れ替え
		animations_.emplace(type, anim);
	}
	else
	{
		// 追加
		animations_[type].model = anim.model;
		animations_[type].animIndex = anim.animIndex;
		animations_[type].attachNo = anim.attachNo;
		animations_[type].totalTime = anim.totalTime;
	}

}

void AnimationController::Add(int type, const float speed, int modelId)
{
	Animation anim;
	if (modelId != -1)
	{
		//リソースマネージャでロードしたものを使う
		anim.model = modelId;
	}
	else
	{
		//持ち主のモデル
		anim.model = modelId_;
	}
	anim.animIndex = type;
	anim.speed = speed;

	//指定番号の配列にアニメーションが存在しない場合
	if (animations_.count(type) == 0)
	{
		//追加
		animations_.emplace(type, anim);
	}
	//存在する場合
	else
	{
		//上書き
		animations_[type].model = anim.model;
		animations_[type].animIndex = anim.animIndex;
		animations_[type].attachNo = anim.attachNo;
		animations_[type].totalTime = anim.totalTime;
	}
}

void AnimationController::Play(int type, bool isLoop,
	float startStep, float endStep, float blendAnimTime, bool isStop, bool isForce, bool isReverse)
{
	//同じ種類かつ強制再生を行わない場合
	if (type == playType_ && !isForce)
	{
		return;
	}
	//if (playType_ != -1)
	//{
	//	// モデルからアニメーションを外す
	//	playAnim_.attachNo = MV1DetachAnim(modelId_, playAnim_.attachNo);
	//}

	// アニメーション種別を変更
	playType_ = type;
	blendAnimRate_ = 0.0f;
	//playAnim_ = animations_[type];

	//再生中のアニメーション情報がある場合
	if (playAnimations_.find(playType_) != playAnimations_.end())
	{
		return;
	}
	Animation anim = animations_[playType_];

	//ブレンドアニメーション時間の設定
	blendAnimTime_ = blendAnimTime;
	// 初期化
	anim.step = startStep;

	// モデルにアニメーションを付ける
	int animIdx = 0;
	if (MV1GetAnimNum(anim.model) > 1)
	{
		// アニメーションが複数保存されていたら、番号1を指定
		animIdx = 1;
	}
	if (modelId_ == anim.model)
	{
		animIdx = type;
	}

	anim.attachNo = MV1AttachAnim(modelId_, animIdx, anim.model);

	// アニメーション総時間の取得
	if (endStep > 0.0f)
	{
		anim.totalTime = endStep;
	}
	else
	{
		anim.totalTime = MV1GetAttachAnimTotalTime(modelId_, anim.attachNo);
	}

	// アニメーションループ
	isLoop_ = isLoop;

	// アニメーションしない
	isStop_ = isStop;


	//再生中のアニメーション配列が空の場合
	if (playAnimations_.empty())
	{
		//進行率は最大にしておく
		anim.blendRate = 1.0f;
	}

	// ブレンドアニメーションを追加
	playAnimations_.emplace(playType_, anim);

	stepEndLoopStart_ = -1.0f;
	stepEndLoopEnd_ = -1.0f;
	switchLoopReverse_ = isReverse ? -1.0f : 1.0f;
}

void AnimationController::Update(void)
{
	//停止してる場合
	if (isStop_)
	{
		//処理は実行しない
		return;
	}

	// メインアニメーション更新
	UpdateMainAnimation();

	// ブレンドアニメーション更新
	UpdateBlendAnimation();
}

void AnimationController::SetEndLoop(float startStep, float endStep, float speed)
{
	stepEndLoopStart_ = startStep;
	stepEndLoopEnd_ = endStep;
	endLoopSpeed_ = speed;
}

int AnimationController::GetPlayType(void) const
{
	return playType_;
}

bool AnimationController::IsEnd(void) const
{	
	auto it = playAnimations_.find(playType_);
	//再生中のアニメーション情報がない場合
	if (it == playAnimations_.end())
	{
		return false;
	}

	//再生中のアニメーション情報を取得
	//Animation& playAnim = playAnimations_[playType_];
	//Animation& playAnim = playAnimations_.at(playType_);
	const Animation& playAnim = it->second;

	if (isLoop_)
	{
		// ループ設定されているなら、
		// 無条件で終了しないを返す
		return true;
	}

	if (playAnim.step >= playAnim.totalTime)
	{
		// 再生時間を過ぎたらtrue
		return true;
	}

	return false;

}

void AnimationController::DebugDraw()
{
	int marginY = 20;
	int index = 0;
	float rate = 0.0f;

	for (auto it = playAnimations_.begin(); it != playAnimations_.end(); )
	{
		//std::wstring type = Utility::GetWStringFromString(it->first);
		int type = it->first;
		DrawFormatString(0, 30 + marginY, 0xff0000, "animType:%d", type);
		DrawFormatString(150, 30 + marginY, 0xff0000, "attachNo:%d", it->second.attachNo);
		DrawFormatString(250, 30 + marginY, 0xff0000, "blendRate:%2f", it->second.blendRate);
		marginY += 20;
		rate += it->second.blendRate;
		it++;
	}
	//合計
	DrawFormatString(0, 0, 0xff0000, "合計:%2f", rate);
	DrawFormatString(0, 100, 0xff0000, "再生中のアタッチNo:%d", playAnimations_[playType_].attachNo);
}

void AnimationController::UpdateMainAnimation()
{
	//再生中のアニメーション情報を取得
	Animation& playAnim = playAnimations_[playType_];

	// 経過時間の取得
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();
	//再生
	playAnim.step += (deltaTime * playAnim.speed * switchLoopReverse_);

	// アニメーション終了判定
	bool isEnd = false;
	if (switchLoopReverse_ > 0.0f)
	{
		// 通常再生の場合
		if (playAnim.step > playAnim.totalTime)
		{
			isEnd = true;
		}
	}
	else
	{
		// 逆再生の場合
		if (playAnim.step < playAnim.totalTime)
		{
			isEnd = true;
		}
	}

	if (isEnd)
	{
		// アニメーションが終了したら
		if (isLoop_)
		{
			// ループ再生
			if (stepEndLoopStart_ > 0.0f)
			{
				// アニメーション終了後の指定フレーム再生
				switchLoopReverse_ *= -1.0f;
				if (switchLoopReverse_ > 0.0f)
				{
					playAnim.step = stepEndLoopStart_;
					playAnim.totalTime = stepEndLoopEnd_;
				}
				else
				{
					playAnim.step = stepEndLoopEnd_;
					playAnim.totalTime = stepEndLoopStart_;
				}
				playAnim.speed = endLoopSpeed_;
			}
			else
			{
				// 通常のループ再生
				playAnim.step = 0.0f;
			}
		}
		else
		{
			// ループしない
			playAnim.step = playAnim.totalTime;
		}
	}

	// 再生するアニメーション時間の設定
	MV1SetAttachAnimTime(modelId_, playAnim.attachNo, playAnim.step);
}

void AnimationController::UpdateBlendAnimation()
{
	// 再生中のアニメーションが一定以上登録されているなら
	if (static_cast<int>(playAnimations_.size()) <= 1)
	{
		return;
	}

	// 経過時間の取得
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();
	// ブレンドアニメーション率を増加
	blendAnimRate_ += deltaTime;

	if (blendAnimRate_ >= blendAnimTime_)
	{
		blendAnimRate_ = blendAnimTime_;
	}

	//ブレンド進行率を計算
	float blendRate = blendAnimRate_ / blendAnimTime_;

	// 登録されているブレンドアニメーション率を更新
	for (auto it = playAnimations_.begin(); it != playAnimations_.end(); )
	{
		//変更後のアニメーションの場合
		if (it->first == playType_)
		{
			//ブレンドアニメーション率を増加
			it->second.blendRate += (1.0f - it->second.blendRate) * blendRate;

			//アニメーションのアタッチ
			MV1SetAttachAnimBlendRate(modelId_, it->second.attachNo, it->second.blendRate);
		}
		//変更前のアニメーションの場合
		else
		{
			//ブレンドアニメーション率を減少
			it->second.blendRate -= it->second.blendRate * blendRate;

			// ブレンドアニメーション率が0以下になったら
			if (it->second.blendRate <= 0.0f)
			{
				//アニメーションのデタッチ
				it->second.attachNo = MV1DetachAnim(modelId_, it->second.attachNo);

				// ブレンドアニメーション率が0以下になったら、リストから削除
				it = playAnimations_.erase(it);
				continue;
			}
			//アニメーションのアタッチ
			MV1SetAttachAnimBlendRate(modelId_, it->second.attachNo, it->second.blendRate);
		}
		++it;
	}
}
