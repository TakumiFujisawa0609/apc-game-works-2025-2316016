#include <DxLib.h>
#include <string>
#include "../../Utility/Utility.h"
#include "../../Application.h"
#include "../../Manager/KeyConfig.h"
#include "../../Manager/ResourceManager.h"
#include "Control.h"

Control::Control() :ins_(KeyConfig::GetInstance())
{
	changeControlType_[CONTROL_TYPE::ENTER] = std::bind(&Control::ChangeControlTypeEnter, this);
	changeControlType_[CONTROL_TYPE::AVOID] = std::bind(&Control::ChangeControlTypeAvoid, this);
	changeControlType_[CONTROL_TYPE::JUMP] = std::bind(&Control::ChangeControlTypeJump, this);
	changeControlType_[CONTROL_TYPE::ATTACK] = std::bind(&Control::ChangeControlTypeAttack, this);
	//changeControlType_[CONTROL_TYPE::LOCK_ON_TYPE] = std::bind(&Control::ChangeControlTypeLockOnType, this);
	changeControlType_[CONTROL_TYPE::LOCK_ON] = std::bind(&Control::ChangeControlTypeLockOn, this);
	changeControlType_[CONTROL_TYPE::MAX] = std::bind(&Control::ChangeControlTypeMax, this);
	ChangeControlType(CONTROL_TYPE::ENTER);

	lastPushBtn_ = KeyConfig::JOYPAD_BTN::MAX;
	enterKey_ = ENTER_KEY::PAD_BTN_RIGHT;
	lastChangeNum_ = LAST_CHANGE_NUM::NONE;
	lastChangeType_ = CONTROL_TYPE::MAX;
	isEnd_ = false;

	notButton.push_back(KeyConfig::JOYPAD_BTN::LEFT_STICK);
	notButton.push_back(KeyConfig::JOYPAD_BTN::RIGHT_STICK);

	KeyConfig::GetInstance().AllClear();
}

Control::~Control()
{
}

void Control::Init(void)
{
}

void Control::Update(void)
{
	updateControlType_();
}

void Control::Draw(void)
{
	if (controlType_ == CONTROL_TYPE::ENTER)
	{
		switch (state_)
		{
		case Control::STATE::CHOOSE:
			//決定時のみ2択だから
			Utility::DrawStringPlace("決定 : B \n キャンセル : A", Application::SCREEN_HALF_X / 2, Application::SCREEN_HALF_Y, 0xffffff, Utility::STRING_PLACE::CENTER);
			Utility::DrawStringPlace("決定 : A \n キャンセル : B", Application::SCREEN_HALF_X / 2 + Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y, 0xffffff, Utility::STRING_PLACE::CENTER);
			switch (enterKey_)
			{
			case Control::ENTER_KEY::PAD_BTN_RIGHT:
				DrawBox(10, 10, Application::SCREEN_HALF_X - 10, Application::SCREEN_SIZE_Y - 10, 0xffff00, false);
				break;
			case Control::ENTER_KEY::PAT_BTN_DOWN:
				DrawBox(Application::SCREEN_HALF_X + 10, 10, Application::SCREEN_SIZE_X - 10, Application::SCREEN_SIZE_Y - 10, 0xffff00, false);
				break;
			default:
				break;
			}
			break;
		case Control::STATE::CHECK:
		{
			std::string msg = "決定 : ";
			msg += enterKey_ == ENTER_KEY::PAD_BTN_RIGHT ? "Bボタン" : "Aボタン";
			msg += " キャンセル : ";
			msg += enterKey_ == ENTER_KEY::PAD_BTN_RIGHT ? "Aボタン" : "Bボタン";
			msg += "でよろしいですか？";
			Utility::DrawStringPlace(msg, Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y, 0xffffff, Utility::STRING_PLACE::CENTER);
		}
		break;
		case Control::STATE::DECIDE:
			break;
		default:
			break;
		}
		Utility::DrawStringPlace("A.B :決定　, X.Y :キャンセル", Application::SCREEN_SIZE_X - 10, Application::SCREEN_SIZE_Y - 20, 0xffffff, Utility::STRING_PLACE::RIGHT);
	}
	else if (controlType_ == CONTROL_TYPE::MAX)
	{
		//最後の確認
		switch (state_)
		{
		case Control::STATE::CHOOSE:
		{
			if (lastChangeNum_ == LAST_CHANGE_NUM::NONE)
			{
				Utility::DrawStringPlace("どの操作を変更しますか？", Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y - 30 * 3, 0xffffff, Utility::STRING_PLACE::CENTER);
				std::string str = "";
				//str += lastChangeType_ == CONTROL_TYPE::ENTER ? "->" : "";
				//str += "決定 : " + GetBtnName(ins_.GetControlBTN(KeyConfig::CONTROL_TYPE::ENTER)) + "\n";
				//str += lastChangeType_ == CONTROL_TYPE::ENTER ? "->" : "";
				//str += "キャンセル : " + GetBtnName(ins_.GetControlBTN(KeyConfig::CONTROL_TYPE::CANCEL)) + "\n";
				str += lastChangeType_ == CONTROL_TYPE::AVOID ? "->" : "";
				str += "回避 : ";
				for (auto btn : ins_.GetControlBTN(KeyConfig::CONTROL_TYPE::PLAYER_AVOID))
				{

					str += Utility::GetBtnName(btn) + " , ";
				}
				str += "\n";
				str += lastChangeType_ == CONTROL_TYPE::JUMP ? "->" : "";
				str += "ジャンプ : ";	
				for (auto btn : ins_.GetControlBTN(KeyConfig::CONTROL_TYPE::PLAYER_JUMP))
				{

					str += Utility::GetBtnName(btn) + " , ";
				}
				str += "\n";
				str += lastChangeType_ == CONTROL_TYPE::ATTACK ? "->" : "";
				str += "攻撃 : ";
				for (auto btn : ins_.GetControlBTN(KeyConfig::CONTROL_TYPE::PLAYER_ATTACK))
				{

					str += Utility::GetBtnName(btn) + " , ";
				}
				str += "\n";
				str += lastChangeType_ == CONTROL_TYPE::LOCK_ON ? "->" : "";
				str += "ロックオン : ";
				for (auto btn : ins_.GetControlBTN(KeyConfig::CONTROL_TYPE::CHENGE_CAMERA_MODE))
				{

					str += Utility::GetBtnName(btn) + " , ";
				}
				str += "\n";
				str += lastChangeType_ == CONTROL_TYPE::MAX ? "->" : "";
				str += "すべて変更";
				Utility::DrawStringPlace(str, Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y - 30 * 2, 0xffffff, Utility::STRING_PLACE::CENTER);
			}
			else
			{
				std::string msg = "";
				switch (lastChangeType_)
				{
				case Control::CONTROL_TYPE::ENTER:
					msg += "決定 : ";
					break;
				case Control::CONTROL_TYPE::AVOID:
					msg += "回避 : ";
					break;
				case Control::CONTROL_TYPE::JUMP:
					msg += "ジャンプ : ";
					break;
				case Control::CONTROL_TYPE::ATTACK:
					msg += "攻撃 : ";
					break;
				case Control::CONTROL_TYPE::LOCK_ON:
					msg += "ロックオン : ";
					break;
				case Control::CONTROL_TYPE::MAX:
					break;
				default:
					break;
				}

				msg += Utility ::GetBtnName(lastPushBtn_);
				Utility::DrawStringPlace(msg, Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y, 0xffffff, Utility::STRING_PLACE::CENTER);
				if (lastPushBtn_ != KeyConfig::JOYPAD_BTN::MAX)
				{
					std::string subMsg = "決定の場合もう一度" + Utility::GetBtnName(lastPushBtn_) + "を押してください";
					Utility::DrawStringPlace(subMsg, Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y + 30, 0xffffff, Utility::STRING_PLACE::CENTER);
				}
			}
		}
		break;
		case Control::STATE::CHECK:
		{
			std::string str = "";
			str += "決定 : ";
			for (auto btn : ins_.GetControlBTN(KeyConfig::CONTROL_TYPE::ENTER))
			{
				str += Utility::GetBtnName(btn) + " , ";
			}
			str += "\n";
			str += "キャンセル : ";
			for (auto btn : ins_.GetControlBTN(KeyConfig::CONTROL_TYPE::CANCEL))
			{
				str +=Utility::GetBtnName(btn) + " , ";
			}
			str += "\n";
			str += "回避 : ";
			for (auto btn : ins_.GetControlBTN(KeyConfig::CONTROL_TYPE::PLAYER_AVOID))
			{
				str += Utility::GetBtnName(btn) + " , ";
			}
			str += "\n";
			str += "ジャンプ : ";
			for (auto btn : ins_.GetControlBTN(KeyConfig::CONTROL_TYPE::PLAYER_JUMP))
			{
				str+=Utility::GetBtnName(btn) + " , ";
			}
			str += "\n";
			str += "攻撃 : ";
			for (auto btn : ins_.GetControlBTN(KeyConfig::CONTROL_TYPE::PLAYER_ATTACK))
			{
				str+=Utility::GetBtnName(btn) + " , ";
			}
			str += "\n";
			str += "ロックオン : ";
			for (auto btn : ins_.GetControlBTN(KeyConfig::CONTROL_TYPE::CHENGE_CAMERA_MODE))
			{
				str+=Utility::GetBtnName(btn) + " , ";
			}
			str += "\n";
			str += "でよろしいですか？";
			Utility::DrawStringPlace(str, Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y - 30 * 3, 0xffffff, Utility::STRING_PLACE::CENTER);

		}
		break;
		case Control::STATE::DECIDE:
			break;
		default:
			break;
		}

	}
	else
	{
		std::string msg = "";
		switch (controlType_)
		{
		case Control::CONTROL_TYPE::ENTER:
			msg += "決定 : ";
			break;
		case Control::CONTROL_TYPE::AVOID:
			msg += "回避 : ";
			break;
		case Control::CONTROL_TYPE::JUMP:
			msg += "ジャンプ : ";
			break;
		case Control::CONTROL_TYPE::ATTACK:
			msg += "攻撃 : ";
			break;
		case Control::CONTROL_TYPE::LOCK_ON:
			msg += "ロックオン : ";
			break;
		case Control::CONTROL_TYPE::MAX:
			break;
		default:
			break;
		}
		msg += Utility::GetBtnName(lastPushBtn_);
		switch (state_)
		{
		case Control::STATE::CHOOSE:
			Utility::DrawStringPlace(msg, Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y, 0xffffff, Utility::STRING_PLACE::CENTER);
			if (lastPushBtn_ != KeyConfig::JOYPAD_BTN::MAX)
			{
				std::string subMsg = "決定の場合もう一度" + Utility::GetBtnName(lastPushBtn_) + "を押してください";
				Utility::DrawStringPlace(subMsg, Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y + 30, 0xffffff, Utility::STRING_PLACE::CENTER);
			}
			break;
		case Control::STATE::CHECK:
			msg += "でよろしいですか？";
			Utility::DrawStringPlace(msg, Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y, 0xffffff, Utility::STRING_PLACE::CENTER);
			break;
		case Control::STATE::DECIDE:
			break;
		default:
			break;
		}

	}
}

void Control::ChangeControlType(CONTROL_TYPE type)
{
	controlType_ = type;
	state_ = STATE::CHOOSE;
	changeControlType_[type]();
	lastPushBtn_ = KeyConfig::JOYPAD_BTN::MAX;
}

void Control::ChangeControlTypeEnter(void)
{
	updateControlType_ = std::bind(&Control::UpdateControlTypeEnter, this);
}

void Control::ChangeControlTypeAvoid(void)
{
	updateControlType_ = std::bind(&Control::UpdateControlTypeAvoid, this);
}

void Control::ChangeControlTypeJump(void)
{
	updateControlType_ = std::bind(&Control::UpdateControlTypeJump, this);
}

void Control::ChangeControlTypeAttack(void)
{
	updateControlType_ = std::bind(&Control::UpdateControlTypeAttack, this);
}

void Control::ChangeControlTypeLockOnType(void)
{
	updateControlType_ = std::bind(&Control::UpdateControlTypeLockOnType, this);
}

void Control::ChangeControlTypeLockOn(void)
{
	updateControlType_ = std::bind(&Control::UpdateControlTypeLockOn, this);
}

void Control::ChangeControlTypeMax(void)
{
	updateControlType_ = std::bind(&Control::UpdateControlTypeMax, this);
	state_ = STATE::CHECK;
}

void Control::UpdateControlTypeEnter(void)
{
	switch (state_)
	{
	case Control::STATE::CHOOSE:
		if (ins_.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_LEFT, KeyConfig::JOYPAD_NO::PAD1) || ins_.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_RIGHT, KeyConfig::JOYPAD_NO::PAD1))
		{
			enterKey_ = static_cast<ENTER_KEY>((static_cast<int>(enterKey_) + 1) % 2);
		}
		if (ins_.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
		{
			state_ = STATE::CHECK;
		}
		break;
	case Control::STATE::CHECK:
		if (ins_.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
		{
			SetControlType(controlType_, enterKey_ == ENTER_KEY::PAD_BTN_RIGHT ? KeyConfig::JOYPAD_BTN::RIGHTBUTTON_RIGHT : KeyConfig::JOYPAD_BTN::RIGHTBUTTON_DOWN);
			SetControlType(KeyConfig::CONTROL_TYPE::CANCEL, enterKey_ == ENTER_KEY::PAD_BTN_RIGHT ? KeyConfig::JOYPAD_BTN::RIGHTBUTTON_DOWN : KeyConfig::JOYPAD_BTN::RIGHTBUTTON_RIGHT);
			state_ = STATE::DECIDE;
		}
		else if (ins_.IsTrgDown(KeyConfig::CONTROL_TYPE::CANCEL, KeyConfig::JOYPAD_NO::PAD1))
		{
			state_ = STATE::CHOOSE;
		}
		break;
	case Control::STATE::DECIDE:
		ChangeControlType(CONTROL_TYPE::AVOID);
		break;
	default:
		break;
	}
}

void Control::UpdateControlTypeAvoid(void)
{
	switch (state_)
	{
	case Control::STATE::CHOOSE:
		ChooseUpdate();
		break;
	case Control::STATE::CHECK:
		CheckUpdate();
		break;
	case Control::STATE::DECIDE:
		ChangeControlType(CONTROL_TYPE::JUMP);
		break;
	default:
		break;
	}
}

void Control::UpdateControlTypeJump(void)
{
	switch (state_)
	{
	case Control::STATE::CHOOSE:
		ChooseUpdate();
		break;
	case Control::STATE::CHECK:
		CheckUpdate();
		break;
	case Control::STATE::DECIDE:
		ChangeControlType(CONTROL_TYPE::ATTACK);
		break;
	default:
		break;
	}
}

void Control::UpdateControlTypeAttack(void)
{
	switch (state_)
	{
	case Control::STATE::CHOOSE:
		ChooseUpdate();
		break;
	case Control::STATE::CHECK:
		CheckUpdate();
		break;
	case Control::STATE::DECIDE:
		ChangeControlType(CONTROL_TYPE::LOCK_ON);
		break;
	default:
		break;
	}
}

void Control::UpdateControlTypeLockOnType(void)
{
}

void Control::UpdateControlTypeLockOn(void)
{
	switch (state_)
	{
	case Control::STATE::CHOOSE:
		ChooseUpdate();
		break;
	case Control::STATE::CHECK:
		CheckUpdate();
		break;
	case Control::STATE::DECIDE:
		ChangeControlType(CONTROL_TYPE::MAX);
		break;
	default:
		break;
	}
}

void Control::UpdateControlTypeMax(void)
{
	switch (state_)
	{
	case Control::STATE::CHOOSE:
		if (lastChangeNum_ == LAST_CHANGE_NUM::NONE)
		{
			if (ins_.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_DOWN, KeyConfig::JOYPAD_NO::PAD1))
			{
				lastChangeType_ = static_cast<CONTROL_TYPE>((static_cast<int>(lastChangeType_) + 1) % (static_cast<int>(CONTROL_TYPE::MAX)));
				lastChangeType_ = lastChangeType_ == CONTROL_TYPE::ENTER ? CONTROL_TYPE::MAX : lastChangeType_;
			}
			else if (ins_.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_UP, KeyConfig::JOYPAD_NO::PAD1))
			{
				lastChangeType_ = static_cast<CONTROL_TYPE>((static_cast<int>(lastChangeType_) - 1) % (static_cast<int>(CONTROL_TYPE::MAX)));
				lastChangeType_ = lastChangeType_ == CONTROL_TYPE::ENTER ? CONTROL_TYPE::MAX : lastChangeType_;
			}

			if (ins_.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
			{
				switch (lastChangeType_)
				{
				case Control::CONTROL_TYPE::ENTER:
					ins_.Clear(KeyConfig::CONTROL_TYPE::ENTER);
					lastChangeNum_ = LAST_CHANGE_NUM::ONE;
					break;
				case Control::CONTROL_TYPE::AVOID:
					ins_.Clear(KeyConfig::CONTROL_TYPE::PLAYER_AVOID);
					lastChangeNum_ = LAST_CHANGE_NUM::ONE;
					break;
				case Control::CONTROL_TYPE::JUMP:
					ins_.Clear(KeyConfig::CONTROL_TYPE::PLAYER_JUMP);
					lastChangeNum_ = LAST_CHANGE_NUM::ONE;
					break;
				case Control::CONTROL_TYPE::ATTACK:
					ins_.Clear(KeyConfig::CONTROL_TYPE::PLAYER_ATTACK);
					lastChangeNum_ = LAST_CHANGE_NUM::ONE;
					break;
				case Control::CONTROL_TYPE::LOCK_ON:
					ins_.Clear(KeyConfig::CONTROL_TYPE::CHENGE_CAMERA_MODE);
					lastChangeNum_ = LAST_CHANGE_NUM::ONE;
					break;
				case Control::CONTROL_TYPE::MAX:
					ins_.AllClear();
					ChangeControlType(CONTROL_TYPE::ENTER);
					lastChangeNum_ = LAST_CHANGE_NUM::ALL;
					break;
				default:
					break;
				}
			}
		}
		else
		{
			LastChangeControlType(lastChangeType_);
		}
		break;
	case Control::STATE::CHECK:
		if (ins_.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
		{
			isEnd_ = true;
		}
		if (ins_.IsTrgDown(KeyConfig::CONTROL_TYPE::CANCEL, KeyConfig::JOYPAD_NO::PAD1))
		{
			//キャンセルでどこを変えるか選ぶ
			state_ = STATE::CHOOSE;
			lastChangeNum_ = LAST_CHANGE_NUM::NONE;
			lastChangeType_ = CONTROL_TYPE::MAX;
		}
		break;
	case Control::STATE::DECIDE:
		break;
	default:
		break;
	}
}

void Control::LastChangeControlType(CONTROL_TYPE type)
{
	auto pushBTN = KeyConfig::GetInstance().GetPushBtns(KeyConfig::JOYPAD_NO::PAD1);
	if (!pushBTN.empty())
	{
		for (auto btn : notButton)
		{
			if (btn == pushBTN[0])
			{
				return;
			}
		}
		if (lastPushBtn_ == pushBTN[0] && pushBTN[0] != KeyConfig::JOYPAD_BTN::MAX)
		{
			//同じボタンが押されたらそれでいいか確認を出す
			state_ = STATE::CHECK;
			SetControlType(type, lastPushBtn_);
			return;
		}
		lastPushBtn_ = pushBTN[0];
	}
}

void Control::SetControlType(CONTROL_TYPE type, KeyConfig::JOYPAD_BTN lastPushBtn)
{
	auto& ins = KeyConfig::GetInstance();
	KeyConfig::CONTROL_TYPE controlType;
	switch (type)
	{
	case Control::CONTROL_TYPE::ENTER:
		controlType = KeyConfig::CONTROL_TYPE::ENTER;
		break;
	case Control::CONTROL_TYPE::AVOID:
		controlType = KeyConfig::CONTROL_TYPE::PLAYER_AVOID;
		break;
	case Control::CONTROL_TYPE::JUMP:
		controlType = KeyConfig::CONTROL_TYPE::PLAYER_JUMP;
		break;
	case Control::CONTROL_TYPE::ATTACK:
		controlType = KeyConfig::CONTROL_TYPE::PLAYER_ATTACK;
		break;
	//case Control::CONTROL_TYPE::LOCK_ON_TYPE:
	//	//controlType = KeyConfig::CONTROL_TYPE::CHENGE_CAMERA_MODE;
	//	break;
	case Control::CONTROL_TYPE::LOCK_ON:
		controlType = KeyConfig::CONTROL_TYPE::CHENGE_CAMERA_MODE;
		break;
	case Control::CONTROL_TYPE::MAX:
		break;
	default:
		break;
	}
	ins.Add(controlType, lastPushBtn);
}
void Control::SetControlType(KeyConfig::CONTROL_TYPE type, KeyConfig::JOYPAD_BTN lastPushBtn)
{
	auto& ins = KeyConfig::GetInstance();
	ins.Add(type, lastPushBtn);
}

void Control::ChooseUpdate(void)
{
	auto pushBTN = KeyConfig::GetInstance().GetPushBtns(KeyConfig::JOYPAD_NO::PAD1);
	if (!pushBTN.empty())
	{
		for (auto btn : notButton)
		{
			if (btn == pushBTN[0])
			{
				return;
			}
		}
		if (lastPushBtn_ == pushBTN[0] && pushBTN[0] != KeyConfig::JOYPAD_BTN::MAX)
		{
			//同じボタンが押されたらそれでいいか確認を出す
			state_ = STATE::CHECK;
			return;
		}
		lastPushBtn_ = pushBTN[0];
	}
}

void Control::CheckUpdate(void)
{
	if (ins_.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
	{
		SetControlType(controlType_, lastPushBtn_);
		state_ = STATE::DECIDE;
	}
	else if (ins_.IsTrgDown(KeyConfig::CONTROL_TYPE::CANCEL, KeyConfig::JOYPAD_NO::PAD1))
	{
		state_ = STATE::CHOOSE;
	}
}
