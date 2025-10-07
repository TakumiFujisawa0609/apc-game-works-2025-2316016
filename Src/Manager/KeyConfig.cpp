#include <cmath>
#include "KeyConfig.h"
#include "InputManager.h"
KeyConfig* KeyConfig::instance_ = nullptr;

void KeyConfig::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new KeyConfig();
	}
	instance_->Init();
}

KeyConfig& KeyConfig::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		KeyConfig::CreateInstance();
	}
	return *instance_;
}

void KeyConfig::Init(void)
{
	inputManager_ = std::make_unique<InputManager>();
	inputManager_->Init();

	//決定処理
	AddFixed(CONTROL_TYPE::ENTER, KEY_INPUT_SPACE);
	AddFixed(CONTROL_TYPE::ENTER, KEY_INPUT_RETURN);
	AddFixed(CONTROL_TYPE::ENTER, MOUSE::CLICK_LEFT);
	AddFixed(CONTROL_TYPE::ENTER, JOYPAD_BTN::RIGHTBUTTON_RIGHT);

	//カメラ変更
	AddFixed(CONTROL_TYPE::CHENGE_CAMERA_MODE, MOUSE::CLICK_RIGHT);
	
	//フェーズ遷移
	AddFixed(CONTROL_TYPE::PHASE_CHENGE, KEY_INPUT_Z);
	AddFixed(CONTROL_TYPE::PHASE_CHENGE, JOYPAD_BTN::START_BUTTON);
	AddFixed(CONTROL_TYPE::PHASE_CHENGE_CHECK, MOUSE::CLICK_LEFT);
	AddFixed(CONTROL_TYPE::PHASE_CHENGE_CHECK, KeyConfig::JOYPAD_BTN::RIGHTBUTTON_RIGHT);

	//プレイ中のカメラ動作
	AddFixed(CONTROL_TYPE::PLAY_CAMERA_MOVE_UP, KEY_INPUT_UP);
	//AddFixed(CONTROL_TYPE::PLAY_CAMERA_MOVE_UP, MOUSE::MOVE_UP);
	AddFixed(CONTROL_TYPE::PLAY_CAMERA_MOVE_DOWN, KEY_INPUT_DOWN);
	//AddFixed(CONTROL_TYPE::PLAY_CAMERA_MOVE_DOWN, MOUSE::MOVE_DOWN);
	AddFixed(CONTROL_TYPE::PLAY_CAMERA_MOVE_RIGHT, KEY_INPUT_RIGHT);
	//AddFixed(CONTROL_TYPE::PLAY_CAMERA_MOVE_RIGHT, MOUSE::MOVE_RIGHT);
	AddFixed(CONTROL_TYPE::PLAY_CAMERA_MOVE_LEFT, KEY_INPUT_LEFT);
	//AddFixed(CONTROL_TYPE::PLAY_CAMERA_MOVE_LEFT, MOUSE::MOVE_LEFT);
	AddFixed(CONTROL_TYPE::PLAY_CAMERA_ZOOM_IN, JOYPAD_BTN::R_TRIGGER);
	AddFixed(CONTROL_TYPE::PLAY_CAMERA_ZOOM_IN, MOUSE::WHEEL_FRONT);
	AddFixed(CONTROL_TYPE::PLAY_CAMERA_ZOOM_OUT, JOYPAD_BTN::L_TRIGGER);
	AddFixed(CONTROL_TYPE::PLAY_CAMERA_ZOOM_OUT, MOUSE::WHEEL_BACK);

	AddFixed(CONTROL_TYPE::PLAYER_MOVE_UP, KEY_INPUT_W);
	AddFixed(CONTROL_TYPE::PLAYER_MOVE_DOWN, KEY_INPUT_S);
	AddFixed(CONTROL_TYPE::PLAYER_MOVE_RIGHT, KEY_INPUT_D);
	AddFixed(CONTROL_TYPE::PLAYER_MOVE_LEFT, KEY_INPUT_A);
	AddFixed(CONTROL_TYPE::PLAYER_JUMP, KEY_INPUT_SPACE);
	AddFixed(CONTROL_TYPE::PLAYER_AVOID, KEY_INPUT_LSHIFT);
	AddFixed(CONTROL_TYPE::PLAYER_ATTACK, MOUSE::CLICK_LEFT);
}

void KeyConfig::Update(void)
{
	inputManager_->Update();
}

bool KeyConfig::IsNew(CONTROL_TYPE cType, JOYPAD_NO no,TYPE type)
{
	if (type == TYPE::KEYBORD_MOUSE || type == TYPE::ALL)
	{
		for (auto& key : fixedKeyInput_)
		{
			if (key.first != cType)
			{
				continue;
			}
			for (auto keyI : key.second)
			{
				if (inputManager_->IsNew(keyI))
				{
					return true;
				}
			}
		}
		for (auto& mouse : fixedMouseInput_)
		{
			if (mouse.first != cType)
			{
				continue;
			}
			for (auto mouseI : mouse.second)
			{
				if (inputManager_->IsMouseNew(mouseI))
				{
					return true;
				}
			}
		}
		for (auto& key : keyInput_)
		{
			if (key.first != cType)
			{
				continue;
			}
			for (auto keyI : key.second)
			{
				if (inputManager_->IsNew(keyI))
				{
					return true;
				}
			}
		}
		for (auto& mouse : mouseInput_)
		{
			if (mouse.first != cType)
			{
				continue;
			}
			for (auto mouseI : mouse.second)
			{
				if (inputManager_->IsMouseNew(mouseI))
				{
					return true;
				}
			}
		}
	}
	if (type == TYPE::PAD || type == TYPE::ALL)
	{
		for (auto& con : fixedConInput_)
		{
			if (con.first != cType)
			{
				continue;
			}
			for (auto conI : con.second)
			{
				if (inputManager_->IsPadBtnNew(no, conI))
				{
					return true;
				}
			}
		}
		for (auto& stick : fixedStickInput_)
		{
			if (stick.first != cType)
			{
				continue;
			}
			for (auto stickI : stick.second)
			{
				if (inputManager_->IsStickNew(no, stickI))
				{
					return true;
				}
			}
		}
		for (auto& con : conInput_)
		{
			if (con.first != cType)
			{
				continue;
			}
			for (auto conI : con.second)
			{
				if (inputManager_->IsPadBtnNew(no, conI))
				{
					return true;
				}
			}
		}
		for (auto& stick : stickInput_)
		{
			if (stick.first != cType)
			{
				continue;
			}
			for (auto stickI : stick.second)
			{
				if (inputManager_->IsStickNew(no, stickI))
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool KeyConfig::IsTrgDown(CONTROL_TYPE cType, JOYPAD_NO no ,TYPE type)
{

	if (type == TYPE::KEYBORD_MOUSE || type == TYPE::ALL)
	{
		for (auto& key : fixedKeyInput_)
		{
			if (key.first != cType)
			{
				continue;
			}
			for (auto keyI : key.second)
			{
				if (inputManager_->IsTrgDown(keyI))
				{
					return true;
				}
			}
		}
		for (auto& mouse : fixedMouseInput_)
		{
			if (mouse.first != cType)
			{
				continue;
			}
			for (auto mouseI : mouse.second)
			{
				if (inputManager_->IsMouseTrgDown(mouseI))
				{
					return true;
				}
			}
		}
		for (auto& key : keyInput_)
		{
			if (key.first != cType)
			{
				continue;
			}
			for (auto keyI : key.second)
			{
				if (inputManager_->IsTrgDown(keyI))
				{
					return true;
				}
			}
		}
		for (auto& mouse : mouseInput_)
		{
			if (mouse.first != cType)
			{
				continue;
			}
			for (auto mouseI : mouse.second)
			{
				if (inputManager_->IsMouseTrgDown(mouseI))
				{
					return true;
				}
			}
		}
	}
	if (type == TYPE::PAD || type == TYPE::ALL)
	{
		for (auto& con : fixedConInput_)
		{
			if (con.first != cType)
			{
				continue;
			}
			for (auto conI : con.second)
			{
				if (inputManager_->IsPadBtnTrgDown(no, conI))
				{
					return true;
				}
			}
		}
		for (auto& stick : fixedStickInput_)
		{
			if (stick.first != cType)
			{
				continue;
			}
			for (auto stickI : stick.second)
			{
				if (inputManager_->IsStickDown(no, stickI))
				{
					return true;
				}
			}
		}
		for (auto& con : conInput_)
		{
			if (con.first != cType)
			{
				continue;
			}
			for (auto conI : con.second)
			{
				if (inputManager_->IsPadBtnTrgDown(no, conI))
				{
					return true;
				}
			}
		}
		for (auto& stick : stickInput_)
		{
			if (stick.first != cType)
			{
				continue;
			}
			for (auto stickI : stick.second)
			{
				if (inputManager_->IsStickDown(no, stickI))
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool KeyConfig::IsTrgUp(CONTROL_TYPE cType, JOYPAD_NO no, TYPE type)
{
	if (type == TYPE::KEYBORD_MOUSE || type == TYPE::ALL)
	{
		for (auto& key : fixedKeyInput_)
		{
			if (key.first != cType)
			{
				continue;
			}
			for (auto keyI : key.second)
			{
				if (inputManager_->IsTrgUp(keyI))
				{
					return true;
				}
			}
		}
		for (auto& mouse : fixedMouseInput_)
		{
			if (mouse.first != cType)
			{
				continue;
			}
			for (auto mouseI : mouse.second)
			{
				if (inputManager_->IsMouseTrgUp(mouseI))
				{
					return true;
				}
			}
		}
		for (auto& key : keyInput_)
		{
			if (key.first != cType)
			{
				continue;
			}
			for (auto keyI : key.second)
			{
				if (inputManager_->IsTrgUp(keyI))
				{
					return true;
				}
			}
		}
		for (auto& mouse : mouseInput_)
		{
			if (mouse.first != cType)
			{
				continue;
			}
			for (auto mouseI : mouse.second)
			{
				if (inputManager_->IsMouseTrgUp(mouseI))
				{
					return true;
				}
			}
		}
	}
	if (type == TYPE::PAD || type == TYPE::ALL)
	{
		for (auto& con : fixedConInput_)
		{
			if (con.first != cType)
			{
				continue;
			}
			for (auto conI : con.second)
			{
				if (inputManager_->IsPadBtnTrgUp(no, conI))
				{
					return true;
				}
			}
		}
		for (auto& stick : fixedStickInput_)
		{
			if (stick.first != cType)
			{
				continue;
			}
			for (auto stickI : stick.second)
			{
				if (inputManager_->IsStickUp(no, stickI))
				{
					return true;
				}
			}
		}
		for (auto& con : conInput_)
		{
			if (con.first != cType)
			{
				continue;
			}
			for (auto conI : con.second)
			{
				if (inputManager_->IsPadBtnTrgUp(no, conI))
				{
					return true;
				}
			}
		}
		for (auto& stick : stickInput_)
		{
			if (stick.first != cType)
			{
				continue;
			}
			for (auto stickI : stick.second)
			{
				if (inputManager_->IsStickUp(no, stickI))
				{
					return true;
				}
			}
		}
	}
	return false;
}

void KeyConfig::Add(CONTROL_TYPE type, int key)
{
	for (auto& keys : keyInput_)
	{
		if (keys.first != type)
		{
			continue;
		}
		keys.second.emplace_back(key);
		return;
	}

	std::vector<int> keys;
	keys.emplace_back(key);
	keyInput_.emplace(type, keys);
}

void KeyConfig::Add(CONTROL_TYPE type, JOYPAD_BTN key)
{
	for (auto& con : conInput_)
	{
		if (con.first != type)
		{
			continue;
		}
		con.second.emplace_back(key);
		return;
	}

	std::vector<JOYPAD_BTN> cons;
	cons.emplace_back(key);
	conInput_.emplace(type, cons);
}

void KeyConfig::Add(CONTROL_TYPE type, JOYPAD_STICK key)
{
	for (auto& stick : stickInput_)
	{
		if (stick.first != type)
		{
			continue;
		}
		stick.second.emplace_back(key);
		return;
	}

	std::vector<JOYPAD_STICK> sticks;
	sticks.emplace_back(key);
	stickInput_.emplace(type, sticks);
}

void KeyConfig::Add(CONTROL_TYPE type, MOUSE key)
{
	for (auto& mouse : mouseInput_)
	{
		if (mouse.first != type)
		{
			continue;
		}
		mouse.second.emplace_back(key);
		return;
	}

	std::vector<MOUSE> mouse;
	mouse.emplace_back(key);
	mouseInput_.emplace(type, mouse);
}

void KeyConfig::AllClear(void)
{
	keyInput_.clear();
	conInput_.clear();
	stickInput_.clear();
	mouseInput_.clear();
}

void KeyConfig::Clear(CONTROL_TYPE type)
{
	keyInput_.erase(type);
	conInput_.erase(type);
	stickInput_.erase(type);
	mouseInput_.erase(type);
}

void KeyConfig::AddFixed(CONTROL_TYPE type,int key )
{
	for (auto &keys : fixedKeyInput_)
	{
		if (keys.first != type)
		{
			continue;
		}
		keys.second.emplace_back(key);
		return;
	}

	std::vector<int> keys;
	keys.emplace_back(key);
	fixedKeyInput_.emplace(type, keys);
}

void KeyConfig::AddFixed(CONTROL_TYPE type,JOYPAD_BTN key)
{
	for (auto& con : fixedConInput_)
	{
		if (con.first != type)
		{
			continue;
		}
		con.second.emplace_back(key);
		return;
	}

	std::vector<JOYPAD_BTN> cons;
	cons.emplace_back(key);
	fixedConInput_.emplace(type, cons);
}

void KeyConfig::AddFixed(CONTROL_TYPE type,JOYPAD_STICK key)
{
	for (auto& stick : fixedStickInput_)
	{
		if (stick.first != type)
		{
			continue;
		}
		stick.second.emplace_back(key);
		return;
	}

	std::vector<JOYPAD_STICK> sticks;
	sticks.emplace_back(key);
	fixedStickInput_.emplace(type, sticks);
}

void KeyConfig::AddFixed(CONTROL_TYPE type, MOUSE key)
{
	for (auto& mouse : fixedMouseInput_)
	{
		if (mouse.first != type)
		{
			continue;
		}
		mouse.second.emplace_back(key);
		return;
	}

	std::vector<MOUSE> mouse;
	mouse.emplace_back(key);
	fixedMouseInput_.emplace(type, mouse);
}


Vector2 KeyConfig::GetMousePos(void) const
{
	return inputManager_->GetMousePos();
}

Vector2 KeyConfig::GetMouseMove(void) const
{
	return inputManager_->GetMousePosDistance();
}

void KeyConfig::SetMousePosScreen(void)
{
	//inputManager_->SetMousePosScreen();
}

void KeyConfig::SetMousePos(const Vector2& pos)
{
	inputManager_->SetMousePos(pos);
}

float KeyConfig::GetLStickDeg(KeyConfig::JOYPAD_NO no) const
{
	float deg = 0.0f;
	Vector2 knockSize = GetKnockLStickSize(no);
	if (knockSize.x == 0.0f && knockSize.y == 0.0f)
	{
		return deg;
	}
	auto rad = std::atan2(knockSize.y, knockSize.x);
	deg = rad * (180.0f / DX_PI_F);
	deg += 90.0f;
	deg = deg < 0 ? deg + 360 : deg;
	return deg;
}

float KeyConfig::GetRStickDeg(KeyConfig::JOYPAD_NO no) const
{
	float deg = 0.0f;
	Vector2 knockSize = GetKnockRStickSize(no);
	if (knockSize.x == 0.0f && knockSize.y == 0.0f)
	{
		return deg;
	}
	auto rad = std::atan2(knockSize.y, knockSize.x);
	deg = rad * (180.0f / DX_PI_F);
	deg += 90.0f;
	deg = deg < 0 ? deg + 360 : deg;
	return deg;
}

Vector2 KeyConfig::GetKnockLStickSize(KeyConfig::JOYPAD_NO no) const
{
	//auto padInfo = padInfos_[static_cast<int>(no)];
	auto padInfo = inputManager_->GetJPadInputState(no);
	return Vector2(padInfo.AKeyLX, padInfo.AKeyLY);
}

Vector2 KeyConfig::GetKnockRStickSize(KeyConfig::JOYPAD_NO no) const
{
	//auto padInfo = padInfos_[static_cast<int>(no)];
	auto padInfo = inputManager_->GetJPadInputState(no);
	return Vector2(padInfo.AKeyRX, padInfo.AKeyRY);
}
int KeyConfig::PadStickOverSize(KeyConfig::JOYPAD_NO no, KeyConfig::JOYPAD_STICK stick) const
{
	int ret = 0;
	auto padInfo = inputManager_->GetJPadInputState(no);
	switch (stick)
	{
	case KeyConfig::JOYPAD_STICK::L_STICK_UP:
		ret = padInfo.AKeyLY < 0 ? padInfo.AKeyLY : 0;
		break;
	case KeyConfig::JOYPAD_STICK::L_STICK_DOWN:
		ret = padInfo.AKeyLY > 0 ? padInfo.AKeyLY : 0;
		break;
	case KeyConfig::JOYPAD_STICK::L_STICK_LEFT:
		ret = padInfo.AKeyLX < 0 ? padInfo.AKeyLX : 0;
		break;
	case KeyConfig::JOYPAD_STICK::L_STICK_RIGHT:
		ret = padInfo.AKeyLX > 0 ? padInfo.AKeyLX : 0;
		break;
	case KeyConfig::JOYPAD_STICK::R_STICK_UP:
		ret = padInfo.AKeyRY < 0 ? padInfo.AKeyRY : 0;
		break;
	case KeyConfig::JOYPAD_STICK::R_STICK_DOWN:
		ret = padInfo.AKeyRY > 0 ? padInfo.AKeyRY : 0;
		break;
	case KeyConfig::JOYPAD_STICK::R_STICK_LEFT:
		ret = padInfo.AKeyRX < 0 ? padInfo.AKeyRX : 0;
		break;
	case KeyConfig::JOYPAD_STICK::R_STICK_RIGHT:
		ret = padInfo.AKeyRX > 0 ? padInfo.AKeyRX : 0;
		break;
	case KeyConfig::JOYPAD_STICK::MAX:
		break;
	default:
		break;
	}
	return abs(ret);
}


void KeyConfig::Destroy(void)
{
	inputManager_->Release();
	fixedKeyInput_.clear();
	fixedConInput_.clear();
	fixedStickInput_.clear();
	fixedMouseInput_.clear();
	delete instance_;
}

void KeyConfig::PadVibration(KeyConfig::JOYPAD_NO _no, int _time, int _pow)
{
	int InputType = _no == JOYPAD_NO::PAD1 ? DX_INPUT_PAD1 :
		_no == JOYPAD_NO::PAD2 ? DX_INPUT_PAD2 :
		_no == JOYPAD_NO::PAD3 ? DX_INPUT_PAD3 : DX_INPUT_PAD4;
	StartJoypadVibration(InputType, _pow, _time);
}

void KeyConfig::StopPadVibration(KeyConfig::JOYPAD_NO _no)
{
	int InputType = _no == JOYPAD_NO::PAD1 ? DX_INPUT_PAD1 :
		_no == JOYPAD_NO::PAD2 ? DX_INPUT_PAD2 :
		_no == JOYPAD_NO::PAD3 ? DX_INPUT_PAD3 : DX_INPUT_PAD4;
	StopJoypadVibration(InputType);
}

KeyConfig::KeyConfig(void)
{
}

KeyConfig::KeyConfig(const KeyConfig& manager)
{
}
