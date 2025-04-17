#include "audio_manager.hpp"

#include "../logger/logger.hpp"
#include "music_groups.hpp"
#include "../resources/resource_accessor.hpp"

#include <format>

AudioManager* globalManager;

void GlobalNextTrack()
{
	globalManager->NextTrack();
}

AudioManager::AudioManager(ResourceAccessor* resources) : resources(resources)
{
	globalManager = this;
}

AudioManager::~AudioManager()
{
	Mix_CloseAudio();
}

void AudioManager::PlaySoundOneShot(const std::string& soundName)
{
	Mix_PlayChannel(-1, resources->Get<Mix_Chunk>(soundName), 0);
}

void AudioManager::StartPlayMusicGroup(const std::string& group)
{
	std::string currentMusicTrack = resources->Get<MusicGroups>("")->GetRandomFromGroup(group);
	currentMusic = resources->Get<Mix_Music>(currentMusicTrack);
	Mix_FadeInMusic(currentMusic, 0, 0);
	Mix_HookMusicFinished(GlobalNextTrack);
	currentMusicGroup = group;
}

void AudioManager::StopPlayingMusic()
{
	Mix_HookMusicFinished(NULL);
	Mix_FadeOutMusic(0);
}

void AudioManager::NextTrack()
{
	StartPlayMusicGroup(currentMusicGroup);
}
