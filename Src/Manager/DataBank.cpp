#include "DataBank.h"

DataBank* DataBank::instance_ = nullptr;

void DataBank::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new DataBank();
	}
	instance_->Init();
}

DataBank& DataBank::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		DataBank::CreateInstance();
	}
	return *instance_;
}

void DataBank::Init(void)
{
	control_ = KeyConfig::TYPE::ALL;
	lockOnType_ = LOCK_ON_TYPE::PRESS; //ƒƒbƒNƒIƒ“‚Ìí—Ş
}

void DataBank::Update(void)
{
}

void DataBank::Destroy(void)
{
	delete instance_;
}

DataBank::DataBank(void)
{
}
