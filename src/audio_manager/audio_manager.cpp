#include "audio_manager.hpp"
#include "../logger/logger.hpp"

#include <format>

AudioManager* globalManager;

void GlobalNextTrack()
{
	globalManager->NextTrack();
}

AudioManager::AudioManager(ResourceContainer* container) : container(container)
{
	globalManager = this;
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
	currentMusicGroup = group;
	currentMusic = container->GetMusicTrack(group);
	Mix_FadeInMusic(currentMusic, 0, 5000);
	Mix_HookMusicFinished(GlobalNextTrack);
}

void AudioManager::StopPlayingMusic()
{
	Mix_FadeOutMusic(1000);
	Mix_HookMusicFinished(NULL);
}

void AudioManager::NextTrack()
{
	StartPlayMusicGroup(currentMusicGroup);
}
