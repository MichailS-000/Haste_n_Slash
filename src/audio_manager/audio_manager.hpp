#pragma once
#include <SDL3_mixer/SDL_mixer.h>
#include "../resources/resource_container.hpp"

#define BG_MUSIC_CHANEL 0

class AudioManager
{
public:
	AudioManager(ResourceContainer* container);
	~AudioManager();
	void PlaySoundOneShot(const std::string& soundName);
	void StartPlayMusicGroup(const std::string& group);
	void StopPlayingMusic();
private:
	ResourceContainer* container;
	Mix_Music* currentPlayngMusic;
};