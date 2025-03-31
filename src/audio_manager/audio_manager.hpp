#pragma once
#include <SDL_mixer.h>
#include "../resources/resource_container.hpp"

#define BG_MUSIC_CHANEL 0

class AudioManager
{
public:
	AudioManager(ResourceContainer* container);
	~AudioManager();
	void PlaySoundOneShot(std::string& soundName);
	void StartPlayLocalMusic(std::string& group);
	void StopPlayingLocalMusic();
private:
	ResourceContainer* container;
	Mix_Music* currentPlayngMusic;
};