#include "Sound.h"

Sound::Sound(void)
{
	soundType_ = TYPE::NONE;
	path_ = "";

	handleId_ = -1;
	pos_ = { 0.0f,0.0f,0.0f };
	radius_ = 0.0f;
	pitch_ = 0.0f;
	maxVolume_ = 255;
	totalTime_ = -1;
}

Sound::Sound(std::shared_ptr<Sound> sound)
{
	soundType_ = sound->soundType_;
	path_ = sound->path_;

	handleId_ = sound->handleId_;
	pos_ = sound->pos_;
	radius_ = 0.0f;
	pitch_ = 0.0f;
	maxVolume_ = 255;
	totalTime_ = -1;
}

Sound::Sound(TYPE type, const std::string& path)
{
	soundType_ = type;
	path_ = path;

	handleId_ = -1;
	pos_ = { 0.0f,0.0f,0.0f };
	radius_ = 0.0f;
	pitch_ = 0.0f;
	maxVolume_ = 255;
	totalTime_ = -1;
}

void Sound::Copy(std::shared_ptr<Sound> sound)
{
	soundType_ = sound->soundType_;
	path_ = sound->path_;
	handleId_ = sound->handleId_;
	pos_ = sound->pos_;
	radius_ = 0.0f;
	pitch_ = 0.0f;
	maxVolume_ = 255;
	totalTime_ = -1;
}

Sound::~Sound(void)
{

}

void Sound::Update(VECTOR pos)
{
	if (soundType_ != TYPE::SOUND_3D || CheckSoundMem(handleId_) != 1)
	{
		//3DˆÈŠO@‚à‚µ‚­‚ÍÄ¶‚µ‚Ä‚¢‚È‚¢ê‡
		return;
	}
	int i = Set3DPositionSoundMem(pos, handleId_);
	if (i == -1)
	{
		return;
	}
	pos_ = pos;
}

void Sound::Load(void)
{
	switch (soundType_)
	{
	case Sound::TYPE::NONE:
		break;
	case Sound::TYPE::SOUND_2D:
		SetCreate3DSoundFlag(false);
		if (pitch_ != 0.0f)
		{
			SetCreateSoundPitchRate(pitch_);
		}
		handleId_ = LoadSoundMem(path_.c_str());
		SetCreateSoundPitchRate(0.0f);

		break;
	case Sound::TYPE::SOUND_3D:
		SetCreate3DSoundFlag(true);
		if (pitch_ != 0.0f)
		{
			SetCreateSoundPitchRate(pitch_);
		}
		handleId_ = LoadSoundMem(path_.c_str());
		SetCreateSoundPitchRate(0.0f);
		break;
	default:
		break;
	}
}


void Sound::Release(void)
{
	if (handleId_ == -1)
	{
		return;
	}
	if (CheckSoundMem(handleId_) == 1)
	{
		StopSoundMem(handleId_);
	}
	DeleteSoundMem(handleId_);
}

bool Sound::Play(TIMES times)
{
	if (soundType_ != TYPE::SOUND_2D || handleId_ == -1)
	{
		return false;	//Ž¸”s
	}
	if (CheckMove())
	{
		return false;
	}
	int i = PlaySoundMem(handleId_, times == TIMES::ONCE ? DX_PLAYTYPE_BACK : DX_PLAYTYPE_LOOP, true);
	ChengeVolume(1.0f);
	return i == 0 ? true : false;
}

bool Sound::Play(VECTOR pos, float radius, TIMES times)
{
	if (soundType_ != TYPE::SOUND_3D || handleId_ == -1)
	{
		return false;	//Ž¸”s
	}
	if (CheckMove())
	{
		return false;
	}
	int i = Set3DPositionSoundMem(pos, handleId_);
	if (i == -1)
	{
		return false;
	}
	i = Set3DRadiusSoundMem(radius, handleId_);
	if (i == -1)
	{
		return false;
	}
	i = PlaySoundMem(handleId_, times == TIMES::ONCE ? DX_PLAYTYPE_BACK : DX_PLAYTYPE_LOOP, true);
	if (i == -1)
	{
		return false;
	}
	pos_ = pos;
	radius_ = radius;
	return true;
}

void Sound::Stop(void)
{
	if (CheckSoundMem(handleId_) != 1)
	{
		return;
	}
	int i = StopSoundMem(handleId_);
	if (i == -1)
	{
		return;
	}
	pos_ = { 0.0f,0.0f,0.0f };
	radius_ = 0.0f;
}

bool Sound::CheckMove(void)
{
	if (CheckSoundMem(handleId_) != 1)
	{
		return false;
	}
	return true;
}

bool Sound::CheckLoad(void)
{
	return handleId_ != -1;
}

void Sound::ChengeVolume(float per)
{
	if (per > 1.0f)
	{
		per = 1.0f;
	}
	if (per < 0.0f)
	{
		per = 0.0f;
	}
	ChangeVolumeSoundMem(static_cast<int>(per * maxVolume_), handleId_);
}

void Sound::ChengeMaxVolume(float per)
{
	maxVolume_ =static_cast<int>(255 * per);
}

void Sound::DuplicateSound(void)
{
	if (handleId_ == -1)
	{
		return;
	}
	//SetCreateSoundPitchRate(pitch_);
	if (GetTotalTime() > THREE_MINIT_MILI_TIME)
	{
		Load();
		return;
	}
	handleId_ = DuplicateSoundMem(handleId_);
	//SetCreateSoundPitchRate(0.0f);
}

void Sound::DuplicateSound(int currentHandle)
{
	if (currentHandle == -1)
	{
		return;
	}
	SetCreateSoundPitchRate(pitch_);
	handleId_ = DuplicateSoundMem(currentHandle);
	SetCreateSoundPitchRate(0.0f);
}

LONGLONG Sound::GetTotalTime(void)
{
	if (totalTime_ > 0)
	{
		return totalTime_;
	}
	totalTime_ = GetSoundTotalTime(handleId_);
	return totalTime_;
}
