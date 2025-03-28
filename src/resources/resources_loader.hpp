#pragma once

#include "../haste_n_slash.hpp"

class ResourcesLoader
{
private:
	SDL_Surface* loadSurface(const std::string& filename);
public:
	StartupOptions LoadStartupOptions();
	ResourcesLoader();
	~ResourcesLoader();
	int LoadResources(ResourceContainer* container);
};