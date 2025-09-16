#pragma once
#include <DxLib.h>
#include <memory>

class Transform;

class PlayerBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name=""></param>
	PlayerBase(void);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~PlayerBase(void);

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name=""></param>
	virtual void Init(void);

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name=""></param>
	virtual void Update(void);

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name=""></param>
	virtual void Draw(void);

protected:

	//基本情報
	std::unique_ptr<Transform> transform_;


private:

};

