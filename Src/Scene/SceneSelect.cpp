#include "../Application.h"
#include "../../Manager/SceneManager.h"
#include "SceneSelect.h"

SceneSelect::SceneSelect(void)
{
	changeSelect_[SELECT::CONTROL_TYPE] = std::bind(&SceneSelect::ChangeSelectControlType, this);
	changeSelect_[SELECT::STAGE_SELECT] = std::bind(&SceneSelect::ChangeSelectStageSelect, this);
	ChangeSelect(SELECT::CONTROL_TYPE);
	lastPushBtn_ = KeyConfig::JOYPAD_BTN::MAX;
	changeControlType_[CONTROL_TYPE::ENTER] = std::bind(&SceneSelect::ChangeControlTypeEnter, this);
	changeControlType_[CONTROL_TYPE::AVOID] = std::bind(&SceneSelect::ChangeControlTypeAvoid, this);
	changeControlType_[CONTROL_TYPE::JUMP] = std::bind(&SceneSelect::ChangeControlTypeJump, this);
	changeControlType_[CONTROL_TYPE::ATTACK] = std::bind(&SceneSelect::ChangeControlTypeAttack, this);
	changeControlType_[CONTROL_TYPE::ROCK_ON] = std::bind(&SceneSelect::ChangeControlTypeRockOn, this);
	changeControlType_[CONTROL_TYPE::MAX] = std::bind(&SceneSelect::ChangeControlTypeMax,this);
	ChangeControlType(CONTROL_TYPE::ENTER);
	enterKey_ = ENTER_KEY::PAD_BTN_RIGHT;
	KeyConfig::GetInstance().AllClear();
}

SceneSelect::~SceneSelect(void)
{
}

bool SceneSelect::Init(void)
{
    return false;
}

void SceneSelect::Update(void)
{
	updateSelect_();
}

void SceneSelect::Draw(void)
{
	drawSelect_();
}

void SceneSelect::ChangeSelect(SELECT select)
{
	select_ = select;
	changeSelect_[select]();
}

void SceneSelect::ChangeSelectControlType(void)
{
	updateSelect_ = std::bind(&SceneSelect::UpdateSelectControlType, this);
	drawSelect_ = std::bind(&SceneSelect::DrawSelectControlType, this);
}

void SceneSelect::ChangeSelectStageSelect(void)
{
	updateSelect_ = std::bind(&SceneSelect::UpdateSelectStageSelect, this);
	drawSelect_ = std::bind(&SceneSelect::DrawSelectStageSelect, this);
}

void SceneSelect::UpdateSelectControlType(void)
{
	updateControlType_();
	if (controlType_ == CONTROL_TYPE::MAX)
	{
		ChangeSelect(SELECT::STAGE_SELECT);
	}
}

void SceneSelect::UpdateSelectStageSelect(void)
{
	if (KeyConfig::GetInstance().IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME, true);
	}
}

void SceneSelect::DrawSelectControlType(void)
{
	DrawFormatString(100, 100, GetColor(255, 255, 255), "lastPush : %d",static_cast<int>(lastPushBtn_));
	if (controlType_ == CONTROL_TYPE::ENTER)
	{
		switch (state_)
		{
		case SceneSelect::STATE::CHOOSE:
			//決定時のみ2択だから
			Utility::DrawStringPlace("決定 : B \n キャンセル : A", Application::SCREEN_HALF_X / 2, Application::SCREEN_HALF_Y, 0xffffff, Utility::STRING_PLACE::CENTER);
			Utility::DrawStringPlace("決定 : A \n キャンセル : B", Application::SCREEN_HALF_X / 2 + Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y, 0xffffff, Utility::STRING_PLACE::CENTER);
			switch (enterKey_)
			{
			case SceneSelect::ENTER_KEY::PAD_BTN_RIGHT:
				DrawBox(10, 10, Application::SCREEN_HALF_X - 10, Application::SCREEN_SIZE_Y - 10, 0xffff00, false);
				break;
			case SceneSelect::ENTER_KEY::PAT_BTN_DOWN:
				DrawBox(Application::SCREEN_HALF_X + 10, 10, Application::SCREEN_SIZE_X - 10, Application::SCREEN_SIZE_Y - 10, 0xffff00, false);
				break;
			default:
				break;
			}
			break;
		case SceneSelect::STATE::CHECK:
		{
			std::string msg = "決定 : ";
			msg += enterKey_ == ENTER_KEY::PAD_BTN_RIGHT ? "Bボタン" : "Aボタン";
			msg += " キャンセル : ";
			msg += enterKey_ == ENTER_KEY::PAD_BTN_RIGHT ? "Aボタン" : "Bボタン";
			msg += "でよろしいですか？";
			Utility::DrawStringPlace(msg, Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y, 0xffffff, Utility::STRING_PLACE::CENTER);
		}
			break;
		case SceneSelect::STATE::DECIDE:
			break;
		default:
			break;
		}
		Utility::DrawStringPlace("A.B :決定　, X.Y :キャンセル", Application::SCREEN_SIZE_X - 10, Application::SCREEN_SIZE_Y - 20, 0xffffff, Utility::STRING_PLACE::RIGHT);
	}
	else
	{
		std::string msg = "";
		switch (controlType_)
		{
		case SceneSelect::CONTROL_TYPE::ENTER:
			msg += "決定 : ";
			break;
		case SceneSelect::CONTROL_TYPE::AVOID:
			msg += "回避 : ";
			break;
		case SceneSelect::CONTROL_TYPE::JUMP:
			msg += "ジャンプ : ";
			break;
		case SceneSelect::CONTROL_TYPE::ATTACK:
			msg += "攻撃 : ";
			break;
		case SceneSelect::CONTROL_TYPE::ROCK_ON:
			msg += "ロックオン : ";
			break;
		case SceneSelect::CONTROL_TYPE::MAX:
			break;
		default:
			break;
		}
		msg += GetBtnName(lastPushBtn_);
		switch (state_)
		{
		case SceneSelect::STATE::CHOOSE:
			Utility::DrawStringPlace(msg, Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y, 0xffffff, Utility::STRING_PLACE::CENTER);
			break;
		case SceneSelect::STATE::CHECK:
			msg += "でよろしいですか？";
			Utility::DrawStringPlace(msg, Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y, 0xffffff, Utility::STRING_PLACE::CENTER);
			break;
		case SceneSelect::STATE::DECIDE:
			break;
		default:
			break;
		}
		std::string str = "";
		auto& ins = KeyConfig::GetInstance();
		str += GetBtnName(ins.GetControlBTN(KeyConfig::CONTROL_TYPE::ENTER)) + " :決定　,";
		str += GetBtnName(ins.GetControlBTN(KeyConfig::CONTROL_TYPE::CANCEL)) + " :キャンセル";
		Utility::DrawStringPlace(str, Application::SCREEN_SIZE_X - 10, Application::SCREEN_SIZE_Y - 20, 0xffffff, Utility::STRING_PLACE::RIGHT);
	}
}

void SceneSelect::DrawSelectStageSelect(void)
{
}

void SceneSelect::ChangeControlType(CONTROL_TYPE type)
{
	controlType_ = type;
	changeControlType_[type]();
	state_ = STATE::CHOOSE;
	lastPushBtn_ = KeyConfig::JOYPAD_BTN::MAX;
}

void SceneSelect::ChangeControlTypeEnter(void)
{
	updateControlType_ = std::bind(&SceneSelect::UpdateControlTypeEnter, this);
}

void SceneSelect::ChangeControlTypeAvoid(void)
{
	updateControlType_ = std::bind(&SceneSelect::UpdateControlTypeAvoid, this);
}

void SceneSelect::ChangeControlTypeJump(void)
{
	updateControlType_ = std::bind(&SceneSelect::UpdateControlTypeJump, this);
}

void SceneSelect::ChangeControlTypeAttack(void)
{
	updateControlType_ = std::bind(&SceneSelect::UpdateControlTypeAttack, this);
}

void SceneSelect::ChangeControlTypeRockOn(void)
{
	updateControlType_ = std::bind(&SceneSelect::UpdateControlTypeRockOn, this);
}

void SceneSelect::ChangeControlTypeMax(void)
{
	updateControlType_ = std::bind(&SceneSelect::UpdateControlTypeMax, this);
}

void SceneSelect::UpdateControlTypeEnter(void)
{
	auto& ins = KeyConfig::GetInstance();
	switch (state_)
	{
	case SceneSelect::STATE::CHOOSE:
		if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_LEFT, KeyConfig::JOYPAD_NO::PAD1) || ins.IsTrgDown(KeyConfig::CONTROL_TYPE::SELECT_RIGHT,KeyConfig::JOYPAD_NO::PAD1))
		{
			enterKey_ = static_cast<ENTER_KEY>((static_cast<int>(enterKey_) + 1) % 2);
		}
		if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER_TEMP,KeyConfig::JOYPAD_NO::PAD1))
		{
			state_ = STATE::CHECK;
		}
		break;
	case SceneSelect::STATE::CHECK:
		if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER_TEMP, KeyConfig::JOYPAD_NO::PAD1))
		{
			SetControlType(controlType_,enterKey_== ENTER_KEY::PAD_BTN_RIGHT?KeyConfig::JOYPAD_BTN::RIGHTBUTTON_RIGHT: KeyConfig::JOYPAD_BTN::RIGHTBUTTON_DOWN);
			SetControlType(KeyConfig::CONTROL_TYPE::CANCEL,enterKey_== ENTER_KEY::PAD_BTN_RIGHT?KeyConfig::JOYPAD_BTN::RIGHTBUTTON_DOWN: KeyConfig::JOYPAD_BTN::RIGHTBUTTON_RIGHT);
			state_ = STATE::DECIDE;
		}
		else if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::CANCEL_TEMP, KeyConfig::JOYPAD_NO::PAD1))
		{
			state_ = STATE::CHOOSE;
		}
		break;
	case SceneSelect::STATE::DECIDE:
		ChangeControlType(CONTROL_TYPE::AVOID);
		break;
	default:
		break;
	}
}

void SceneSelect::UpdateControlTypeAvoid(void)
{
	auto& ins = KeyConfig::GetInstance();
	switch (state_)
	{
	case SceneSelect::STATE::CHOOSE:
	{
		auto pushBTN = KeyConfig::GetInstance().GetPushBtns(KeyConfig::JOYPAD_NO::PAD1);
		if (!pushBTN.empty())
		{
			if (lastPushBtn_ == pushBTN[0] && pushBTN[0] != KeyConfig::JOYPAD_BTN::MAX)
			{
				//同じボタンが押されたらそれでいいか確認を出す
				state_ = STATE::CHECK;
				return;
			}
			lastPushBtn_ = pushBTN[0];
		}
	}
		break;
	case SceneSelect::STATE::CHECK:
		if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
		{
			SetControlType(controlType_,lastPushBtn_);
			state_ = STATE::DECIDE;
		}
		else if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::CANCEL, KeyConfig::JOYPAD_NO::PAD1))
		{
			state_ = STATE::CHOOSE;
		}
		break;
	case SceneSelect::STATE::DECIDE:
		ChangeControlType(CONTROL_TYPE::JUMP);
		break;
	default:
		break;
	}
}

void SceneSelect::UpdateControlTypeJump(void)
{
	auto& ins = KeyConfig::GetInstance();
	switch (state_)
	{
	case SceneSelect::STATE::CHOOSE:
	{
		auto pushBTN = KeyConfig::GetInstance().GetPushBtns(KeyConfig::JOYPAD_NO::PAD1);
		if (!pushBTN.empty())
		{
			if (lastPushBtn_ == pushBTN[0] && pushBTN[0] != KeyConfig::JOYPAD_BTN::MAX)
			{
				//同じボタンが押されたらそれでいいか確認を出す
				state_ = STATE::CHECK;
				return;
			}
			lastPushBtn_ = pushBTN[0];
		}
	}
	break;
	case SceneSelect::STATE::CHECK:
		if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
		{
			SetControlType(controlType_, lastPushBtn_);
			state_ = STATE::DECIDE;
		}
		else if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::CANCEL, KeyConfig::JOYPAD_NO::PAD1))
		{
			state_ = STATE::CHOOSE;
		}
		break;
	case SceneSelect::STATE::DECIDE:
		ChangeControlType(CONTROL_TYPE::ATTACK);
		break;
	default:
		break;
	}
}

void SceneSelect::UpdateControlTypeAttack(void)
{
	auto& ins = KeyConfig::GetInstance();
	switch (state_)
	{
	case SceneSelect::STATE::CHOOSE:
	{
		auto pushBTN = KeyConfig::GetInstance().GetPushBtns(KeyConfig::JOYPAD_NO::PAD1);
		if (!pushBTN.empty())
		{
			if (lastPushBtn_ == pushBTN[0] && pushBTN[0] != KeyConfig::JOYPAD_BTN::MAX)
			{
				//同じボタンが押されたらそれでいいか確認を出す
				state_ = STATE::CHECK;
				return;
			}
			lastPushBtn_ = pushBTN[0];
		}
	}
	break;
	case SceneSelect::STATE::CHECK:
		if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
		{
			SetControlType(controlType_, lastPushBtn_);
			state_ = STATE::DECIDE;
		}
		else if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::CANCEL, KeyConfig::JOYPAD_NO::PAD1))
		{
			state_ = STATE::CHOOSE;
		}
		break;
	case SceneSelect::STATE::DECIDE:
		ChangeControlType(CONTROL_TYPE::ROCK_ON);
		break;
	default:
		break;
	}
}

void SceneSelect::UpdateControlTypeRockOn(void)
{
	auto& ins = KeyConfig::GetInstance();
	switch (state_)
	{
	case SceneSelect::STATE::CHOOSE:
	{
		auto pushBTN = KeyConfig::GetInstance().GetPushBtns(KeyConfig::JOYPAD_NO::PAD1);
		if (!pushBTN.empty())
		{
			if (lastPushBtn_ == pushBTN[0] && pushBTN[0] != KeyConfig::JOYPAD_BTN::MAX)
			{
				//同じボタンが押されたらそれでいいか確認を出す
				state_ = STATE::CHECK;
				return;
			}
			lastPushBtn_ = pushBTN[0];
		}
	}
	break;
	case SceneSelect::STATE::CHECK:
		if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::ENTER, KeyConfig::JOYPAD_NO::PAD1))
		{
			SetControlType(controlType_, lastPushBtn_);
			state_ = STATE::DECIDE;
		}
		else if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::CANCEL, KeyConfig::JOYPAD_NO::PAD1))
		{
			state_ = STATE::CHOOSE;
		}
		break;
	case SceneSelect::STATE::DECIDE:
		ChangeControlType(CONTROL_TYPE::MAX);
		break;
	default:
		break;
	}
}

void SceneSelect::UpdateControlTypeMax(void)
{
	ChangeSelect(SELECT::STAGE_SELECT);
}

void SceneSelect::SetControlType(CONTROL_TYPE type, KeyConfig::JOYPAD_BTN lastPushBtn)
{
	auto& ins = KeyConfig::GetInstance();
	KeyConfig::CONTROL_TYPE controlType;
	switch (type)
	{
	case SceneSelect::CONTROL_TYPE::ENTER:
		controlType = KeyConfig::CONTROL_TYPE::ENTER;
		break;
	case SceneSelect::CONTROL_TYPE::AVOID:
		controlType = KeyConfig::CONTROL_TYPE::PLAYER_AVOID;
		break;
	case SceneSelect::CONTROL_TYPE::JUMP:
		controlType = KeyConfig::CONTROL_TYPE::PLAYER_JUMP;
		break;
	case SceneSelect::CONTROL_TYPE::ATTACK:
		controlType = KeyConfig::CONTROL_TYPE::PLAYER_ATTACK;
		break;
	case SceneSelect::CONTROL_TYPE::ROCK_ON:
		controlType = KeyConfig::CONTROL_TYPE::CHENGE_CAMERA_MODE;
		break;
	case SceneSelect::CONTROL_TYPE::MAX:
		break;
	default:
		break;
	}
	ins.Add(controlType, lastPushBtn);
}
void SceneSelect::SetControlType(KeyConfig::CONTROL_TYPE type, KeyConfig::JOYPAD_BTN lastPushBtn)
{
	auto& ins = KeyConfig::GetInstance();
	ins.Add(type, lastPushBtn);
}

std::string SceneSelect::GetBtnName(KeyConfig::JOYPAD_BTN btn)
{
	std::string name = "";
	switch (btn)
	{
	case KeyConfig::JOYPAD_BTN::RIGHTBUTTON_RIGHT:
		name = "Bボタン";
		break;
	case KeyConfig::JOYPAD_BTN::RIGHTBUTTON_DOWN:
		name = "Aボタン";
		break;
	case KeyConfig::JOYPAD_BTN::RIGHTBUTTON_LEFT:
		name = "Xボタン";
		break;
	case KeyConfig::JOYPAD_BTN::RIGHTBUTTON_TOP:
		name = "Yボタン";
		break;
	case KeyConfig::JOYPAD_BTN::R_TRIGGER:
		name = "Rトリガー";
		break;
	case KeyConfig::JOYPAD_BTN::L_TRIGGER:
		name = "Lトリガー";
		break;
	case KeyConfig::JOYPAD_BTN::R_BUTTON:
		name = "Rボタン";
		break;
	case KeyConfig::JOYPAD_BTN::L_BUTTON:
		name = "Lボタン";
		break;
	case KeyConfig::JOYPAD_BTN::START_BUTTON:
		name = "スタートボタン";
		break;
	case KeyConfig::JOYPAD_BTN::SELECT_BUTTON:
		name = "セレクトボタン";
		break;
	case KeyConfig::JOYPAD_BTN::LEFTBUTTON_TOP:
		name = "十字キー上";
		break;
	case KeyConfig::JOYPAD_BTN::LEFTBUTTON_DOWN:
		name = "十字キー下";
		break;
	case KeyConfig::JOYPAD_BTN::LEFTBUTTON_LEFT:
		name = "十字キー左";
		break;
	case KeyConfig::JOYPAD_BTN::LEFTBUTTON_RIGHT:
		name = "十字キー右";
		break;
	case KeyConfig::JOYPAD_BTN::LEFT_STICK:
		name = "左スティック押し込み";
		break;
	case KeyConfig::JOYPAD_BTN::RIGHT_STICK:
		name = "右スティック押し込み";
		break;
	case KeyConfig::JOYPAD_BTN::MAX:
		name = "割り当てたいボタンを2回押してください";
		break;
	}
	return name;
}
