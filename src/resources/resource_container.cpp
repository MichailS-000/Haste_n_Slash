#include "resource_container.hpp"
#include <format>

#include "../logger/logger.hpp"

ResourceContainer::~ResourceContainer()
{
	for (auto& [textureName, texture] : textures)
	{
		SDL_DestroyTexture(texture);
	}
	for (auto& [sfxName, sfx] : sfxces)
	{
		Mix_FreeChunk(sfx);
	}
	for (auto& [musicName, mus] : music)
	{
		Mix_FreeMusic(mus);
	}
	for (auto& [fontName, font] : fonts)
	{
		TTF_CloseFont(font);
	}
	for (auto& [scriptName, script] : scripts)
	{
		delete script;
	}

	delete startupOptions;
	delete musicGroups;
}
