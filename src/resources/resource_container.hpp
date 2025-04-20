#pragma once

#include <memory>
#include <unordered_map>
#include <format>

#include "resource_types.hpp"
#include "startup_options.hpp"
#include "../audio_manager/music_groups.hpp"
#include "../scripts/compiled_script.hpp"

#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "../logger/logger.hpp"

class ResourceContainer
{
private:
	std::unordered_map<std::string, SDL_Texture*> textures;
	std::unordered_map<std::string, Mix_Chunk*> sfxces;
	std::unordered_map<std::string, Mix_Music*> music;
	std::unordered_map<std::string, TTF_Font*> fonts;
	std::unordered_map<std::string, CompiledScript*> scripts;
	StartupOptions* startupOptions;
	MusicGroups* musicGroups;
public:
	template<typename T>
	void AddResource(std::string name, T* resource)
	{
		if constexpr (std::is_same_v<T, SDL_Texture>)
		{
			Logger::Log(std::format("Texture \"{}\" loaded", name));
			textures[name] = reinterpret_cast<SDL_Texture*>(resource);
		}
		else if constexpr(std::is_same_v<T, Mix_Chunk>)
		{
			Logger::Log(std::format("SFX \"{}\" loaded", name));
			sfxces[name] = reinterpret_cast<Mix_Chunk*>(resource);
		}
		else if constexpr (std::is_same_v<T, Mix_Music>)
		{
			Logger::Log(std::format("Musical track \"{}\" loaded", name));
			music[name] = reinterpret_cast<Mix_Music*>(resource);
		}
		else if constexpr (std::is_same_v<T, TTF_Font>)
		{
			Logger::Log(std::format("Font \"{}\" loaded", name));
			fonts[name] = reinterpret_cast<TTF_Font*>(resource);
		}
		else if constexpr (std::is_same_v<T, CompiledScript>)
		{
			Logger::Log(std::format("Script \"{}\" loaded and compiled", name));
			scripts[name] = reinterpret_cast<CompiledScript*>(resource);
		}
		else if constexpr (std::is_same_v<T, StartupOptions>)
		{
			Logger::Log(std::format("Startup options \"{}\" loaded", name));
			startupOptions = reinterpret_cast<StartupOptions*>(resource);
		}
		else if constexpr (std::is_same_v<T, MusicGroups>)
		{
			Logger::Log(std::format("Music groups \"{}\" loaded", name));
			musicGroups = reinterpret_cast<MusicGroups*>(resource);
		}
		else
		{
			Logger::LogWarning(1, std::format("Unknown resource type \"{}\" of \"{}\"", typeid(T).name(), name));
		}
	}

	template<typename T>
	T* GetResource(std::string name)
	{
		if constexpr (std::is_same_v<T, SDL_Texture>)
		{
			return textures[name];
		}
		else if constexpr (std::is_same_v<T, Mix_Chunk>)
		{
			return sfxces[name];
		}
		else if constexpr (std::is_same_v<T, Mix_Music>)
		{
			return music[name];
		}
		else if constexpr (std::is_same_v<T, TTF_Font>)
		{
			return fonts[name];
		}
		else if constexpr (std::is_same_v<T, CompiledScript>)
		{
			return scripts[name];
		}
		else if constexpr (std::is_same_v<T, StartupOptions>)
		{
			return startupOptions;
		}
		else if constexpr (std::is_same_v<T, MusicGroups>)
		{
			return musicGroups;
		}
		else
		{
			Logger::LogWarning(std::format("Unknown resource type \"{}\" of \"{}\"", typeid(T).name(), name));
		}
	}

	~ResourceContainer();
};