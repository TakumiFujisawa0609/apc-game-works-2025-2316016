#pragma once
#include <DxLib.h>
#include <map>
#include <string>
class KeyMap
{
public:
	static void CreateInstance(void);
	static KeyMap& GetInstance(void);
	void Init(void);
	void Destroy(void);
	std::string GetKeyName(int key);
private:
	std::map<int, std::string> keyMap_;
	static KeyMap* instance_;
	KeyMap();
	~KeyMap() = default;
};

