#include <DxLib.h>
#include "../../Application.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/KeyConfig.h"
#include "../../Manager/SceneManager.h"
#include "PadUI.h"

PadUI::PadUI(void)
{
	size_ = { 0,0 };
	state_ = STATE::HIDE;
	changeVec_ = { static_cast<float>(PAD_IMAGE_SIZE_X / CHANGE_TIME),static_cast<float>(PAD_IMAGE_SIZE_Y / CHANGE_TIME) };
	changeState_[STATE::SHOW] = std::bind(&PadUI::ChangeToShow, this);
	changeState_[STATE::HIDE] = std::bind(&PadUI::ChangeToHide, this);
	changeState_[STATE::ENLARGE] = std::bind(&PadUI::ChangeToEnlarge, this);
	changeState_[STATE::REDUCATE] = std::bind(&PadUI::ChangeToReducte, this);
	ChangeState(STATE::HIDE);
}

PadUI::~PadUI(void)
{
}

void PadUI::Init(void)
{

}

void PadUI::Update(void)
{
	stateUpdate_();
}

void PadUI::Draw(void)
{
	if (state_ != STATE::HIDE)
	{
		DrawExtendGraph(Application::SCREEN_SIZE_X - size_.x, Application::SCREEN_SIZE_Y - size_.y,Application::SCREEN_SIZE_X,Application::SCREEN_SIZE_Y, ResourceManager::GetInstance().Load(ResourceManager::SRC::PAD_UI_IMG).handleId_, true);
	}
}

void PadUI::ChangeState(STATE newState)
{
	state_ = newState;
	changeState_[state_]();
}

void PadUI::ChangeToShow(void)
{
	stateUpdate_ = std::bind(&PadUI::ShowUpdate, this);
}

void PadUI::ChangeToHide(void)
{
	stateUpdate_ = std::bind(&PadUI::HideUpdate, this);
}

void PadUI::ChangeToEnlarge(void)
{
	time_ = CHANGE_TIME;
	stateUpdate_ = std::bind(&PadUI::EnlargeUpdate, this);
}

void PadUI::ChangeToReducte(void)
{
	time_ = CHANGE_TIME;
	stateUpdate_ = std::bind(&PadUI::ReducteUpdate, this);
}

void PadUI::ShowUpdate(void)
{
	KeyConfig& ins = KeyConfig::GetInstance();
	if(ins.IsTrgDown(KeyConfig::CONTROL_TYPE::PAD_SHOW_HIDE, KeyConfig::JOYPAD_NO::PAD1))
	{
		ChangeState(STATE::REDUCATE);
	}
}

void PadUI::HideUpdate(void)
{
	KeyConfig& ins = KeyConfig::GetInstance();
	if (ins.IsTrgDown(KeyConfig::CONTROL_TYPE::PAD_SHOW_HIDE, KeyConfig::JOYPAD_NO::PAD1))
	{
		ChangeState(STATE::ENLARGE);
	}
}

void PadUI::EnlargeUpdate(void)
{
	time_ -= SceneManager::GetInstance().GetDeltaTime();
	if (time_ <= 0.0f)
	{
		time_ = 0.0f;
		size_ = { PAD_IMAGE_SIZE_X,PAD_IMAGE_SIZE_Y };
		ChangeState(STATE::SHOW);
	}
	else
	{
		size_.x += changeVec_.u * SceneManager::GetInstance().GetDeltaTime();
		size_.y += changeVec_.v * SceneManager::GetInstance().GetDeltaTime();
	}
}

void PadUI::ReducteUpdate(void)
{
	time_ -= SceneManager::GetInstance().GetDeltaTime();
	if (time_ <= 0.0f)
	{
		time_ = 0.0f;
		size_ = { 0,0 };
		ChangeState(STATE::HIDE);
	}
	else
	{
		size_.x -= changeVec_.u * SceneManager::GetInstance().GetDeltaTime();
		size_.y -= changeVec_.v * SceneManager::GetInstance().GetDeltaTime();
	}
}
