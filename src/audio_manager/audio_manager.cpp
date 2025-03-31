#include "audio_manager.hpp"

AudioManager::AudioManager(ResourceContainer* container) : container(container)
{
	SDL_Init(SDL_INIT_AUDIO);
}

AudioManager::~AudioManager()
{
	Mix_CloseAudio();
}

void AudioManager::PlaySoundOneShot(std::string& soundName)
{
	Mix_PlayChannel(-1, container->GetSound(soundName), 0);
}

void AudioManager::StartPlayLocalMusic(std::string& music)
{
}

void AudioManager::StopPlayingLocalMusic()
{
}
