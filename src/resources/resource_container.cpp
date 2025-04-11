#include "resource_container.hpp"
#include <format>

#include "../logger/logger.hpp"

void ResourceContainer::AddImage(Image& img)
{
	images.push(new Image(img));
	Logger::Log(std::format("Image \"{}\" loaded", img.name));
}

void ResourceContainer::AddFont(const std::string& fontFilename, const std::string& fontName)
{
	TTF_Font* font = TTF_OpenFont(fontFilename.c_str(), 24);
	
	if (font == NULL)
	{
		Logger::LogError(4, std::format("Error while loading \"{}\" font {}", fontFilename, SDL_GetError()));
	}

	fonts.emplace(fontName, font);
	Logger::Log(std::format("Font \"{}\" loaded", fontName));
}

void ResourceContainer::AddUncompiledScript(UncompiledScript& script)
{
	uncompiledScripts.push(new UncompiledScript(script));
	Logger::Log(std::format("Script \"{}\" loaded", script.name));
}

void ResourceContainer::AddMusic(Mix_Music* musicTrack, const std::string& groupName)
{
	musicGroups[groupName].push_back(musicTrack);

	Logger::Log(std::format("New track added to group \"{}\"", groupName));
}

void ResourceContainer::AddSound(Mix_Chunk* sound, const std::string& soundName)
{
	if (sounds.contains(soundName))
	{
		Logger::LogWarning(4, std::format("Sound\"{}\" already exists", soundName));
	}
	else
	{
		sounds.emplace(soundName, sound);
	}

	Logger::Log(std::format("SFX \"{}\" loaded", soundName));
}

Image* ResourceContainer::GetNextImage()
{
	if (images.size() > 0)
	{
		Image* img = new Image(*images.front());
		delete images.front();
		images.pop();
		return img;
	}
	else
	{
		return nullptr;
	}
}

UncompiledScript* ResourceContainer::GetNextUncompiledScript()
{
	if (uncompiledScripts.size() > 0)
	{
		UncompiledScript* script = new UncompiledScript(*uncompiledScripts.front());
		delete uncompiledScripts.front();
		uncompiledScripts.pop();
		return script;
	}
	else
	{
		return nullptr;
	}
}

Mix_Music* ResourceContainer::GetMusicTrack(const std::string& groupName)
{
	if (musicGroups[groupName].size() > 0)
	{
		int randomTrack = rand() % musicGroups[groupName].size();
		return musicGroups[groupName][randomTrack];
	}
	else
	{
		Logger::LogError(4, std::format("Group {} is empty", groupName));
		return nullptr;
	}
}

Mix_Chunk* ResourceContainer::GetSound(const std::string& soundName)
{
	if (sounds.contains(soundName))
	{
		return sounds[soundName];
	}
	else
	{
		Logger::LogError(4, std::format("Sound {} does not exists", soundName));
		return nullptr;
	}
}

TTF_Font* ResourceContainer::GetFont(const std::string& fontName)
{
	return fonts[fontName];
}

ResourceContainer::ResourceContainer()
{
	if (!TTF_Init())
	{
		Logger::LogError(4, "TTF initialization failed!");
	}
}

ResourceContainer::~ResourceContainer()
{
	Mix_FadeOutMusic(1000);
	for (auto& [groupName, group] : musicGroups)
	{
		for (auto track : group)
		{
			Mix_FreeMusic(track);
		}
	}
	for (auto& [soundName, sound] : sounds)
	{
		Mix_FreeChunk(sound);
	}
	for (auto& [fontName, font] : fonts)
	{
		TTF_CloseFont(font);
	}

	TTF_Quit();
}
