#pragma once

#include "../haste_n_slash.hpp"
#include <string>
#include "resource_container.hpp"

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