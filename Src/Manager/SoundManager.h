#pragma once
#include<map>
#include <vector>
#include<memory>
#include"Sound.h"

class SoundManager
{
public:
	//リソース名
	enum class SRC
	{
		TITLE_BGM,		//タイトルシーンのBGM
		GAME_BGM,	//げーむのBGM	

		ENTER_SOUND,	//決定音

		PSHOT_HIT,	//プレイヤーの球の衝突音
		PSHOT_THROW,	//プレイヤーの球を投げる音
		FIRE,	//炎
		JAMP,	//ジャンプ
		LAND,	//着地
		THUNDER,	//雷
		MAX

	};
	enum class SOUND_TYPE
	{
		BGM,
		SE,
	};
	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static SoundManager& GetInstance(void);

	// 初期化
	void Init(void);

	// 解放(シーン切替時に一旦解放)
	void Release(void);

	// リソースの完全破棄
	void Destroy(void);

	/// <summary>
	/// 二次元音源再生
	/// </summary>
	/// <param name="src">音の種類</param>
	/// <param name="times">１回かループか</param>
	/// <returns>再生が成功したらtrue</returns>
	bool Play(SRC src,Sound::TIMES times);

	/// <summary>
	/// 三次元音源再生
	/// </summary>
	/// <param name="src">音の種類</param>
	/// <param name="times">１回かループか</param>
	/// <param name="pos">再生する場所</param>
	/// <param name="radius">音が聞こえる範囲</param>
	/// <returns>再生成功したらtrue</returns>
	bool Play(SRC src,Sound::TIMES times,VECTOR pos, float radius);

	/// <summary>
	/// 指定の音をすべて停止する
	/// </summary>
	/// <param name="src">音の種類</param>
	void Stop(SRC src);

	/// <summary>
	/// 指定の音が1つでもなっているかどうか
	/// </summary>
	/// <param name="src">音の種類</param>
	/// <returns>１つでもなっていたらtrue</returns>
	bool CheckMove(SRC src);

	/// <summary>
	/// 指定した音の音量を全て変える
	/// </summary>
	/// <param name="src">音の種類</param>
	/// <param name="per">0.0～1.0でパーセントを設定</param>
	void ChangeVolume(SRC src, float per);

	/// <summary>
	/// BGMかSE全ての音量を変える
	/// </summary>
	/// <param name="">BGM か SE</param>
	/// <param name="per">0.0～1.0でパーセントを設定</param>
	void ChangeVolume(SOUND_TYPE type, float per);

	/// <summary>
	/// 3D音源の耳の位置と前方ベクトルを設定する
	/// </summary>
	/// <param name="pos">耳の位置(カメラ座標)</param>
	/// <param name="frontPos">前方方向(カメラ注視点)</param>
	void Set3DListenPosAndFrontPos(VECTOR pos, VECTOR frontPos);

	/// <summary>
	/// BGMかSEの音量パーセンテージを返す
	/// </summary>
	/// <param name="type">BGMかSE</param>
	/// <returns>0.0～1.0でパーセンテージを返す</returns>
	float GetVolume(SOUND_TYPE type) { return volume_[type]; }


	LONGLONG GetTotalTime(SRC src);

	/// <summary>
	/// 音の前ロード
	/// </summary>
	/// <param name="src">種類</param>
	void Load(SRC src);
private:
	// 静的インスタンス
	static SoundManager* instance_;
	//BGMかSEに分ける
	std::map<SOUND_TYPE, std::vector<SRC>> soundType_;
	//同時最大再生数
	std::map<SRC, int> maxPlayNum;
	//読み込み済みリソース
	std::map<SRC, std::shared_ptr<Sound>> loadMap_;
	//プレイ用
	std::map<SRC, std::vector<std::shared_ptr<Sound>>> playMap_;

	//音量パーセント
	std::map<SOUND_TYPE, float> volume_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	SoundManager(void) = default;

	// デストラクタも同様
	~SoundManager(void) = default;
};

