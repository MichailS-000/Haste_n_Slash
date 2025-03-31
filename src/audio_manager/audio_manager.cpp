#include "audio_manager.hpp"

AudioManager::AudioManager(ResourceContainer* container) : container(container)
{
}

AudioManager::~AudioManager()
{
	Mix_CloseAudio();
}

void AudioManager::PlaySoundOneShot(const std::string& soundName)
{
	Mix_PlayChannel(-1, container->GetSound(soundName), 0);
}

void AudioManager::StartPlayMusicGroup(const std::string& group)
{
	Mix_FadeInMusic(container->GetMusicTrack(group), 0, 5000);
}

void AudioManager::StopPlayingMusic()
{
	Mix_FadeOutMusic(1000);
}
