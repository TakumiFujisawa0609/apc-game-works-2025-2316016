#include <DxLib.h>
#include "../Application.h"
#include "SoundManager.h"

SoundManager* SoundManager::instance_ = nullptr;

void SoundManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new SoundManager();
	}
	instance_->Init();
}

SoundManager& SoundManager::GetInstance(void)
{
	return *instance_;
}

void SoundManager::Init(void)
{
	//BGM‚©SE‚É•ª‚¯‚é
	soundType_[SOUND_TYPE::BGM].push_back(SRC::TITLE_BGM);
	soundType_[SOUND_TYPE::BGM].push_back(SRC::GAME_BGM);
	soundType_[SOUND_TYPE::SE].push_back(SRC::ENTER_SOUND);
	soundType_[SOUND_TYPE::SE].push_back(SRC::PSHOT_HIT);
	soundType_[SOUND_TYPE::SE].push_back(SRC::PSHOT_THROW);
	soundType_[SOUND_TYPE::SE].push_back(SRC::FIRE);
	soundType_[SOUND_TYPE::SE].push_back(SRC::JAMP);
	soundType_[SOUND_TYPE::SE].push_back(SRC::LAND);
	//Å‘åÄ¶”‚ğ‰Šú‰»‚·‚é
	for (int i = 0; i < static_cast<int>(SRC::MAX);i++)
	{
		//‰Šú‰»‚Å‚·‚×‚Ä‚Ì‰¹‚ª“¯‚É1‚Â‚µ‚©‚È‚ç‚È‚¢‚æ‚¤‚É‚·‚é
		maxPlayNum.emplace(static_cast<SRC>(i), 1);
		playMap_.emplace(static_cast<SRC>(i), std::vector<std::shared_ptr<Sound>>{});
	}


	std::shared_ptr<Sound> res;

	res = std::make_unique<Sound>(Sound::TYPE::SOUND_2D, Application::PATH_SOUND_BGM + "TitleBGM.mp3");
	res->ChengeMaxVolume(1.0f);
	loadMap_.emplace(SRC::TITLE_BGM, std::move(res));
	res = std::make_unique<Sound>(Sound::TYPE::SOUND_2D, Application::PATH_SOUND_BGM + "GameBGM.mp3");
	res->ChengeMaxVolume(1.0f);
	loadMap_.emplace(SRC::GAME_BGM, std::move(res));
	res = std::make_unique<Sound>(Sound::TYPE::SOUND_2D, Application::PATH_SOUND_SE + "PShotHit.mp3");
	res->ChengeMaxVolume(1.0f);
	loadMap_.emplace(SRC::PSHOT_HIT, std::move(res));
	res = std::make_unique<Sound>(Sound::TYPE::SOUND_2D, Application::PATH_SOUND_SE + "PShotThrow.mp3");
	res->ChengeMaxVolume(1.0f);
	loadMap_.emplace(SRC::PSHOT_THROW, std::move(res));
	res = std::make_unique<Sound>(Sound::TYPE::SOUND_2D, Application::PATH_SOUND_SE + "Fire.mp3");
	res->ChengeMaxVolume(1.0f);
	loadMap_.emplace(SRC::FIRE, std::move(res));
	res = std::make_unique<Sound>(Sound::TYPE::SOUND_2D, Application::PATH_SOUND_SE + "Jamp.mp3");
	maxPlayNum[SRC::JAMP] = 2;
	res->ChengeMaxVolume(1.0f);
	loadMap_.emplace(SRC::JAMP, std::move(res));
	res = std::make_unique<Sound>(Sound::TYPE::SOUND_2D, Application::PATH_SOUND_SE + "Land.mp3");
	maxPlayNum[SRC::LAND] = 2;
	res->ChengeMaxVolume(1.0f);
	loadMap_.emplace(SRC::LAND, std::move(res));
	res = std::make_unique<Sound>(Sound::TYPE::SOUND_2D, Application::PATH_SOUND_SE + "Thunder.mp3");
	res->ChengeMaxVolume(1.0f);
	loadMap_.emplace(SRC::THUNDER, std::move(res));
	ChangeVolume(SOUND_TYPE::BGM, 0.8f);
	ChangeVolume(SOUND_TYPE::SE, 0.8f);
}

void SoundManager::Release(void)
{
	for (auto& p : loadMap_)
	{
		p.second->Release();
	}

	loadMap_.clear();
	InitSoundMem();
}

void SoundManager::Destroy(void)
{
	Release();
	loadMap_.clear();
	delete instance_;
}

bool SoundManager::Play(SRC src, Sound::TIMES times)
{
	
	const auto& lPair = loadMap_.find(src);
	if (lPair != loadMap_.end())
	{
		if (!lPair->second->CheckLoad())
		{
			lPair->second->Load();
		}
		if (playMap_[src].size() < maxPlayNum[src])
		{
			std::shared_ptr<Sound>sound;
			sound = std::make_shared<Sound>();
			sound->Copy(lPair->second);
			//sound = lPair->second;
			sound->DuplicateSound();
			bool isPlay = sound->Play(times);
			playMap_[src].push_back(sound);

			return isPlay;
		}
		else
		{
			for (auto& plays : playMap_[src])
			{
				if (plays->CheckMove())
				{
					continue;
				}
				if (plays->Play(times))
				{
					return true;
				}
			}
			return false;
		}
	}
	return false;
}

bool SoundManager::Play(SRC src, Sound::TIMES times, VECTOR pos, float radius)
{
	const auto& lPair = loadMap_.find(src);
	if (lPair != loadMap_.end())
	{
		if (!lPair->second->CheckLoad())
		{
			lPair->second->Load();
		}
		if (playMap_[src].size() < maxPlayNum[src])
		{
			std::shared_ptr<Sound>sound;
			sound = std::make_shared<Sound>();
			sound->Copy(lPair->second);
			//sound = lPair->second;
			sound->DuplicateSound();
			bool isPlay = sound->Play(pos, radius, times);
			playMap_[src].push_back(sound);

			return isPlay;
		}
		else
		{
			for (auto& plays : playMap_[src])
			{
				if (plays->CheckMove())
				{
					continue;
				}
				if (plays->Play(pos, radius, times))
				{
					return true;
				}
			}
			return false;
		}
		return lPair->second->Play(pos,radius,times);
	}
	return false;
}

void SoundManager::Stop(SRC src)
{
	const auto& lPair = playMap_.find(src);
	if (lPair != playMap_.end())
	{
		for (auto& sound : lPair->second)
		{
			sound->Stop();
		}
	}
}

bool SoundManager::CheckMove(SRC src)
{
	const auto& lPair = playMap_.find(src);
	if (lPair != playMap_.end())
	{
		for (auto& sound : lPair->second)
		{
			if (sound->CheckMove())
			{
				return true;
			}
		}
	}
	return false;

}

void SoundManager::ChangeVolume(SRC src, float per)
{
	const auto& lPair = playMap_.find(src);
	if (lPair != playMap_.end())
	{
		for (auto& sound : lPair->second)
		{
			sound->ChengeVolume(per);
		}
	}
}

void SoundManager::ChangeVolume(SOUND_TYPE type, float per)
{
	volume_[type] = per;
	for (auto& srcPair : soundType_)
	{
		if (srcPair.first != type)
		{
			continue;
		}
		for (auto src : srcPair.second)
		{
			ChangeVolume(src, per);
		}
	}
}

void SoundManager::Set3DListenPosAndFrontPos(VECTOR pos, VECTOR frontPos)
{
	Set3DSoundListenerPosAndFrontPos_UpVecY(pos, frontPos);
}

LONGLONG SoundManager::GetTotalTime(SRC src)
{
	const auto& lPair = playMap_.find(src);
	if (lPair != playMap_.end())
	{
		for (auto& sound : lPair->second)
		{
			return sound->GetTotalTime();
		}
	}
	return -1;
}

void SoundManager::Load(SRC src)
{
	const auto& lPair = loadMap_.find(src);
	if (lPair != loadMap_.end())
	{
		if (!lPair->second->CheckLoad())
		{
			lPair->second->Load();
		}
	}
}
