#include "KeyMap.h"
	
KeyMap* KeyMap::instance_ = nullptr;

void KeyMap::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new KeyMap();
	}
	instance_->Init();
}
	
KeyMap& KeyMap::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		KeyMap::CreateInstance();
	}
	return *instance_;
}
	
void KeyMap::Init(void)
{

	keyMap_[KEY_INPUT_BACK] = " バックスペースキー";
	keyMap_[KEY_INPUT_TAB] = " タブキー";
	keyMap_[KEY_INPUT_RETURN] = " エンターキー";
	keyMap_[KEY_INPUT_LSHIFT] = " 左シフトキー";
	keyMap_[KEY_INPUT_RSHIFT] = " 右シフトキー";
	keyMap_[KEY_INPUT_LCONTROL] = " 左コントロールキー";
	keyMap_[KEY_INPUT_RCONTROL] = " 右コントロールキー";
	keyMap_[KEY_INPUT_ESCAPE] = " エスケープキー";
	keyMap_[KEY_INPUT_SPACE] = " スペースキー";
	keyMap_[KEY_INPUT_PGUP] = " ＰａｇｅＵＰキー";
	keyMap_[KEY_INPUT_PGDN] = " ＰａｇｅＤｏｗｎキー";
	keyMap_[KEY_INPUT_END] = " エンドキー";
	keyMap_[KEY_INPUT_HOME] = " ホームキー";
	keyMap_[KEY_INPUT_LEFT] = " 左キー";
	keyMap_[KEY_INPUT_UP] = " 上キー";
	keyMap_[KEY_INPUT_RIGHT] = " 右キー";
	keyMap_[KEY_INPUT_DOWN] = " 下キー";
	keyMap_[KEY_INPUT_INSERT] = " インサートキー";
	keyMap_[KEY_INPUT_DELETE] = " デリートキー";
	keyMap_[KEY_INPUT_MINUS] = " －キー";
	keyMap_[KEY_INPUT_YEN] = " ￥キー";
	keyMap_[KEY_INPUT_PREVTRACK] = " ＾キー";
	keyMap_[KEY_INPUT_PERIOD] = " ．キー";
	keyMap_[KEY_INPUT_SLASH] = " ／キー";
	keyMap_[KEY_INPUT_LALT] = " 左ＡＬＴキー";
	keyMap_[KEY_INPUT_RALT] = " 右ＡＬＴキー";
	keyMap_[KEY_INPUT_SCROLL] = " ScrollLockキー";
	keyMap_[KEY_INPUT_SEMICOLON] = " ；キー";
	keyMap_[KEY_INPUT_COLON] = " ：キー";
	keyMap_[KEY_INPUT_LBRACKET] = " ［キー";
	keyMap_[KEY_INPUT_RBRACKET] = " ］キー";
	keyMap_[KEY_INPUT_AT] = " ＠キー";
	keyMap_[KEY_INPUT_BACKSLASH] = " ＼キー";
	keyMap_[KEY_INPUT_COMMA] = " ，キー";
	keyMap_[KEY_INPUT_CAPSLOCK] = " CaspLockキー";
	keyMap_[KEY_INPUT_PAUSE] = " PauseBreakキー";
	keyMap_[KEY_INPUT_NUMPAD0] = " テンキー０";
	keyMap_[KEY_INPUT_NUMPAD1] = " テンキー１";
	keyMap_[KEY_INPUT_NUMPAD2] = " テンキー２";
	keyMap_[KEY_INPUT_NUMPAD3] = " テンキー３";
	keyMap_[KEY_INPUT_NUMPAD4] = " テンキー４";
	keyMap_[KEY_INPUT_NUMPAD5] = " テンキー５";
	keyMap_[KEY_INPUT_NUMPAD6] = " テンキー６";
	keyMap_[KEY_INPUT_NUMPAD7] = " テンキー７";
	keyMap_[KEY_INPUT_NUMPAD8] = " テンキー８";
	keyMap_[KEY_INPUT_NUMPAD9] = " テンキー９";
	keyMap_[KEY_INPUT_MULTIPLY] = " テンキー＊キー";
	keyMap_[KEY_INPUT_ADD] = " テンキー＋キー";
	keyMap_[KEY_INPUT_SUBTRACT] = " テンキー－キー";
	keyMap_[KEY_INPUT_DECIMAL] = " テンキー．キー";
	keyMap_[KEY_INPUT_DIVIDE] = " テンキー／キー";
	keyMap_[KEY_INPUT_NUMPADENTER] = " テンキーのエンターキー";
	keyMap_[KEY_INPUT_F1] = " Ｆ１キー";
	keyMap_[KEY_INPUT_F2] = " Ｆ２キー";
	keyMap_[KEY_INPUT_F3] = " Ｆ３キー";
	keyMap_[KEY_INPUT_F4] = " Ｆ４キー";
	keyMap_[KEY_INPUT_F5] = " Ｆ５キー";
	keyMap_[KEY_INPUT_F6] = " Ｆ６キー";
	keyMap_[KEY_INPUT_F7] = " Ｆ７キー";
	keyMap_[KEY_INPUT_F8] = " Ｆ８キー";
	keyMap_[KEY_INPUT_F9] = " Ｆ９キー";
	keyMap_[KEY_INPUT_F10] = " Ｆ１０キー";
	keyMap_[KEY_INPUT_F11] = " Ｆ１１キー";
	keyMap_[KEY_INPUT_F12] = " Ｆ１２キー";
	keyMap_[KEY_INPUT_A] = " Ａキー";
	keyMap_[KEY_INPUT_B] = " Ｂキー";
	keyMap_[KEY_INPUT_C] = " Ｃキー";
	keyMap_[KEY_INPUT_D] = " Ｄキー";
	keyMap_[KEY_INPUT_E] = " Ｅキー";
	keyMap_[KEY_INPUT_F] = " Ｆキー";
	keyMap_[KEY_INPUT_G] = " Ｇキー";
	keyMap_[KEY_INPUT_H] = " Ｈキー";
	keyMap_[KEY_INPUT_I] = " Ｉキー";
	keyMap_[KEY_INPUT_J] = " Ｊキー";
	keyMap_[KEY_INPUT_K] = " Ｋキー";
	keyMap_[KEY_INPUT_L] = " Ｌキー";
	keyMap_[KEY_INPUT_M] = " Ｍキー";
	keyMap_[KEY_INPUT_N] = " Ｎキー";
	keyMap_[KEY_INPUT_O] = " Ｏキー";
	keyMap_[KEY_INPUT_P] = " Ｐキー";
	keyMap_[KEY_INPUT_Q] = " Ｑキー";
	keyMap_[KEY_INPUT_R] = " Ｒキー";
	keyMap_[KEY_INPUT_S] = " Ｓキー";
	keyMap_[KEY_INPUT_T] = " Ｔキー";
	keyMap_[KEY_INPUT_U] = " Ｕキー";
	keyMap_[KEY_INPUT_V] = " Ｖキー";
	keyMap_[KEY_INPUT_W] = " Ｗキー";
	keyMap_[KEY_INPUT_X] = " Ｘキー";
	keyMap_[KEY_INPUT_Y] = " Ｙキー";
	keyMap_[KEY_INPUT_Z] = " Ｚキー";
	keyMap_[KEY_INPUT_0] = " ０キー";
	keyMap_[KEY_INPUT_1] = " １キー";
	keyMap_[KEY_INPUT_2] = " ２キー";
	keyMap_[KEY_INPUT_3] = " ３キー";
	keyMap_[KEY_INPUT_4] = " ４キー";
	keyMap_[KEY_INPUT_5] = " ５キー";
	keyMap_[KEY_INPUT_6] = " ６キー";
	keyMap_[KEY_INPUT_7] = " ７キー";
	keyMap_[KEY_INPUT_8] = " ８キー";
	keyMap_[KEY_INPUT_9] = " ９キー";
}
	
void KeyMap::Destroy(void)
{
	keyMap_.clear();
	delete instance_;
	instance_ = nullptr;
}

std::string KeyMap::GetKeyName(int key)
{
	auto it = keyMap_.find(key);
	if (it == keyMap_.end())
	{
		return "";
	}
	return it->second;
}
	
KeyMap::KeyMap()
{
}
