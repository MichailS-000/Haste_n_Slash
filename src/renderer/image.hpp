#pragma once
#include <SDL3/SDL.h>
#include <string>

struct Image
{
	std::string name;
	SDL_Surface* surface;
};