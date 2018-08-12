#pragma once
#include <unordered_map>
#include <cassert>
#include <string>

#include <Windows.h>
#include <fmod.hpp>
#include <fmod_errors.h>

#pragma comment(lib, "fmod64_vc.lib")

#include "singletonBase.h"
#include "enum.h"

class SoundManager : public SingletonBase<SoundManager>
{
private:
	FMOD::System* _system;
	//vector<FMOD::Channel*> _channels; //FMOD::Channel* _channel;
	FMOD::Channel* _channels[15];
	bool _check[15];
	std::unordered_map<SoundTypeTag, FMOD::Sound*> _sounds;
	float _volume;

	void ERRCHECK(FMOD_RESULT result)
	{
		if (result != FMOD_OK)
		{
			char str[256];
			sprintf_s(str, "FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
			MessageBox(NULL, str, "TEST", MB_OK);
		}
	}

public:
	void init()
	{
		ERRCHECK(FMOD::System_Create(&_system));
		ERRCHECK(_system->init(12, FMOD_INIT_NORMAL, 0));
		

		/* 사운드 파일 등록 */
		registerSound(SoundTypeTag::Logo, "assets/sound/logo.mp3");
		registerSound(SoundTypeTag::LoginScene, "assets/sound/login_scene.mp3");
		registerSound(SoundTypeTag::ModeSelectScene, "assets/sound/mode_select_scene.mp3");
		registerSound(SoundTypeTag::LobbyScene, "assets/sound/lobby_scene.mp3");
		registerSound(SoundTypeTag::PlayScene, "assets/sound/play_scene.mp3");
	
		registerSound(SoundTypeTag::GameStart, "assets/sound/game_start.mp3");
		registerSound(SoundTypeTag::EatItem, "assets/sound/eat_item.mp3");
		registerSound(SoundTypeTag::Wave, "assets/sound/wave.mp3");

		registerSound(SoundTypeTag::Win, "assets/sound/win.mp3");
		registerSound(SoundTypeTag::Lose, "assets/sound/lose.mp3");

		registerSound(SoundTypeTag::PtInRect, "assets/sound/pt_in_rect.mp3");
		registerSound(SoundTypeTag::Click, "assets/sound/click.mp3");
		
		registerSound(SoundTypeTag::BombPop, "assets/sound/bomb_pop.mp3");
		registerSound(SoundTypeTag::BombSet, "assets/sound/bomb_set.mp3");
		registerSound(SoundTypeTag::GameOver, "assets/sound/game_over.mp3");
		registerSound(SoundTypeTag::PlayerDie, "assets/sound/player_die.mp3");


	}

	void release()
	{
		for (auto sound : _sounds)
			if (sound.second)
				sound.second->release();

		if (_system)
			_system->release();
	}
	void registerSound(const SoundTypeTag& sound, string path)
	{
		auto key = sound;
		auto find = _sounds.find(key);
		if (find == _sounds.end())
			ERRCHECK(_system->createSound(path.c_str(), FMOD_DEFAULT, 0, &_sounds[key]));
		else
			assert(false && "SoundManager::registerSound()");
	}

	void play(int channelKey, const SoundTypeTag& sound)
	{
		//bool isPlay;
		//_channels[channelKey]->isPlaying(&isPlay);
		//if (isPlay)
		//{
		//	return;
		//}
		auto find = _sounds.find(sound);
		if (find != _sounds.end())
			ERRCHECK(_system->playSound(find->second, NULL, false, &_channels[channelKey]));
		else
			assert(false && "Sound::play()");
	}
	void repeatPlay(int channelKey, const SoundTypeTag& sound)
	{		
		bool isPlay;
		_channels[channelKey]->isPlaying(&isPlay);
		if (!isPlay)
		{
			play(channelKey, sound);
		}
	}
	void pause(int channelKey, bool paused)
	{
		_channels[channelKey]->setPaused(paused);
	}

	void update()
	{
		_system->update();
	}

	void stop(int channelKey)
	{
		_channels[channelKey]->stop();
	}

	void setVolume(const float volume)
	{
		if (volume < 0.0f)
			_volume = 0.0f;
		else if (volume > 1.0f)
			_volume = 1.0f;
		else
			_volume = volume;
	}

	float getVolume() const
	{
		return _volume;
	}

	void volumeUp(const float volume)
	{
		setVolume(_volume + volume);
	}

	void volumeDown(const float volume)
	{
		setVolume(_volume - volume);
	}
};
