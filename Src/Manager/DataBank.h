#pragma once
#include "KeyConfig.h"

class DataBank
{
public:
	enum class LOCK_ON_TYPE
	{
		FIXED,	//固定(ずっとロックオン状態)
		PRESS,	//押している間だけロックオン
		SWITCH, //押すたびにロックオンのON/OFF切り替え
	};

	enum class DIFFICULTY
	{
		TUTORIAL,	//チュートリアル
	};

	// インスタンスを明示的に生成
	static void CreateInstance(void);

	// インスタンスの取得
	static DataBank& GetInstance(void);

	void Init(void);
	void Update(void);
	void Destroy(void);
	void SetControlType(KeyConfig::TYPE type) { control_ = type; };
	void SetLockOnType(LOCK_ON_TYPE type) { lockOnType_ = type; }
	KeyConfig::TYPE GetControlType(void) const { return control_; };
	LOCK_ON_TYPE GetLockOnType(void) const { return lockOnType_; };
private:
	
	KeyConfig::TYPE control_;
	LOCK_ON_TYPE lockOnType_; //ロックオンの種類

	static DataBank* instance_;

	DataBank(void);
	~DataBank(void) = default;

};

