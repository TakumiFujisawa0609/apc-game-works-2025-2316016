#pragma once
#include "KeyConfig.h"

class DataBank
{
public:
	enum class LOCKON_MODE
	{
		ALWAYS,	//固定(ずっとロックオン状態)
		HOLD,	//押している間だけロックオン
		TOGGLE, //押すたびにロックオンのON/OFF切り替え
		MAX,
	};

	// インスタンスを明示的に生成
	static void CreateInstance(void);

	// インスタンスの取得
	static DataBank& GetInstance(void);

	void Init(void);
	void Update(void);
	void Destroy(void);
	void SetControlType(KeyConfig::TYPE type) { control_ = type; };
	void SetLockOnMode(LOCKON_MODE type) { lockOnType_ = type; }
	KeyConfig::TYPE GetControlType(void) const { return control_; };
	LOCKON_MODE GetLockOnMode(void) const { return lockOnType_; };
private:
	
	KeyConfig::TYPE control_;
	LOCKON_MODE lockOnType_; //ロックオンの種類

	static DataBank* instance_;

	DataBank(void) = default;
	~DataBank(void) = default;

};

