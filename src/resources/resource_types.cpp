#include "resource_types.hpp"

#include "../scripts/scripts_manager.hpp"
#include "../logger/logger.hpp"

#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>

#include <format>

IResource::IResource(const rapidjson::Value& jsonObj, const std::string& name)
{
	type = jsonObj["type"].GetString();
	this->name = name;
}

ImageResource::ImageResource(const rapidjson::Value& jsonObj, const std::string& name) : IResource(jsonObj, name)
{
	source = jsonObj["source"].GetString();
}

SDL_Texture* ImageResource::Load(SDL_Renderer* renderer)
{
	SDL_Surface* surface = IMG_Load(source.c_str());

	if (surface == nullptr)
	{
		surface = SDL_LoadBMP(source.c_str());
		if (surface == nullptr)
		{
			Logger::LogError(3, std::format("Error occured while loading texture from source \"{}\"\nError:{}", source, SDL_GetError()));
		}
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_PIXELART);
	SDL_DestroySurface(surface);
	return texture;
}

MusicResource::MusicResource(const rapidjson::Value& jsonObj, const std::string& name) : IResource(jsonObj, name)
{
	source = jsonObj["source"].GetString();
	group = jsonObj["group"].GetString();
}

const std::string& MusicResource::GetGroup()
{
	return group;
}

Mix_Music* MusicResource::Load()
{
	Mix_Music* music = Mix_LoadMUS(source.c_str());

	if (music == nullptr)
	{
		Logger::LogError(3, std::format("Error occured while loading music from source \"{}\"\nError:{}", source, SDL_GetError()));
	}

	return music;
}

SoundResource::SoundResource(const rapidjson::Value& jsonObj, const std::string& name) : IResource(jsonObj, name)
{
	source = jsonObj["source"].GetString();
}

Mix_Chunk* SoundResource::Load()
{
	return Mix_LoadWAV(source.c_str());
}

FontResource::FontResource(const rapidjson::Value& jsonObj, const std::string& name) : IResource(jsonObj, name)
{
	source = jsonObj["source"].GetString();
	size = jsonObj["size"].GetInt();
}

TTF_Font* FontResource::Load()
{
	TTF_Font* font = TTF_OpenFont(source.c_str(), size);

	if (font == nullptr)
	{
		Logger::LogError(3, std::format("Error occured while loading font from source \"{}\"\nError:{}", source, SDL_GetError()));
	}

	return font;
}

ScriptResource::ScriptResource(const rapidjson::Value& jsonObj, const std::string& name) : IResource(jsonObj, name)
{
	source = jsonObj["source"].GetString();
	for (auto& permission : jsonObj["permissions"].GetArray())
	{
		permissions.push_back(permission.GetString());
	}
}

CompiledScript* ScriptResource::Load(ScriptsManager* scriptManager)
{
	return scriptManager->CompileScript(source, permissions);
}
