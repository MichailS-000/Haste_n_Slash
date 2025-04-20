#pragma once
#include <SDL3_mixer/SDL_mixer.h>
#include <string>

class ResourceAccessor;

class AudioManager
{
public:
	AudioManager(ResourceAccessor* resources);
	~AudioManager();
	void PlaySoundOneShot(const std::string& soundName);
	void StartPlayMusicGroup(const std::string& group);
	void StopPlayingMusic();
	void NextTrack();
private:
	ResourceAccessor* resources;
	Mix_Music* currentMusic = nullptr;
	std::string currentMusicGroup;
};