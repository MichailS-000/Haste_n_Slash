#pragma once

#include <map>
#include <memory>
#include <queue>

#include "../renderer/image.hpp"
#include "../scripts/uncompiled_script.hpp"
#include <SDL3_mixer/SDL_mixer.h>

class ResourceContainer
{
private:
	std::queue<UncompiledScript*> uncompiledScripts;
	std::queue<Image*> images;
	std::map<std::string, std::vector<Mix_Music*>> musicGroups;
	std::map<std::string, Mix_Chunk*> sounds;
public:
	void AddImage(Image& img);
	void AddUncompiledScript(UncompiledScript& script);
	void AddMusic(Mix_Music* musicTrack, const std::string& groupName);
	void AddSound(Mix_Chunk* sound, const std::string& soundName);

	Image* GetNextImage();
	UncompiledScript* GetNextUncompiledScript();

	Mix_Music* GetMusicTrack(const std::string& groupName);
	Mix_Chunk* GetSound(const std::string& soundName);

	ResourceContainer();
	~ResourceContainer();
};

