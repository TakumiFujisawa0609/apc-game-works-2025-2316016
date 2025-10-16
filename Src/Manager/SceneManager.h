//#pragma once
#include<memory>
#include<list>
#include"../Utility/Utility.h"

class SceneBase;
class Fader;
class Camera;

class SceneManager
{
public:
	
	//列挙型
	enum class SCENE_ID
	{
		NONE
		,TITLE
		,SELECT
		,GAME
		,GAMEOVER
		,GAMECLEAR
		,MENU
		,SETTING
	};
	//メンバー関数
	//-----------------------------------------------------------
	//初期化処理(初回の1度のみ実行される)
	bool Init(void);
	void Init3D(void);
	//更新処理
	void Update(void);

	//描画処理
	void Draw(void);

	//解放処理(終了時の１度のみ実行)
	void Destroy(void);

	// デルタタイムの取得
	float GetDeltaTime(void) const;

	//シーン切り替え用
	void ChangeScene(SCENE_ID nextID,bool isToFade);

	/// <summary>
	/// シーンを新しく「積む」
	/// </summary>
	/// <param name="scene"></param>
	void PushScene(SCENE_ID pushId);

	/// <summary>
	/// 最後に追加したシーンを削除する
	/// </summary>
	void PopScene();

	/// <summary>
	/// 強制的に特定のシーンに飛ぶ。積んでてもリセットされる
	/// </summary>
	/// <param name="scene">ジャンプ先シーン</param>
	void JumpScene(SCENE_ID id);
	//シングルトン化
	//------------------------------------------------

	//インスタンスの生成
	static void CreateInstance(void);
	//外部でインスタンスを利用するため
	static SceneManager& GetInstance(void);

	Camera& GetCamera(void) { return *camera_; }

	int GetMainScreen(void) { return mainScreen_; }
private:
	//メンバー変数
	int nowPushSpace;           //今スペースを押しているか
	int oldPushSpace;           //1フレーム前スペースを押してるか

	SCENE_ID sceneID_;                  //現在のしーんID
	SCENE_ID nextSceneID_;              //次のシーンID

	bool isSceneChanging_;             //シーン遷移中かの判断用フラグ(true:遷移中)

	int mainScreen_;

	//フェード
	std::unique_ptr<Fader> fader_;                   //フェードのインスタンス

	std::list<std::unique_ptr<SceneBase>> scenes_;

	std::unique_ptr<Camera> camera_; //カメラインスタンス

	//メンバー関数
	//void DoChangeScene(void); //シーンを切り替える
	void Fade(void);          //フェード実行用関数
	//void ReleaseScene(void);  //シーン解放関数

	//コピーコンストラクタを利用できなくする
	SceneManager(const SceneManager& ins);


	//静的インスタンス
	static SceneManager* instance_;


	SceneManager(void);

	~SceneManager(void);

	float deltaTime_;

	std::unique_ptr<SceneBase> MakeScene(SCENE_ID id);
};