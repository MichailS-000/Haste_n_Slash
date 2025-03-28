#pragma once

#include "startup_options.hpp"
#include "resource_container.hpp"

class ResourcesLoader
{
private:
	SDL_Surface* loadSurface(const std::string& filename);
	/// <summary>
	/// Returns integer resolution by pointers from string
	/// </summary>
	/// <param name="resolution">String resolution</param>
	/// <param name="widthPtr"></param>
	/// <param name="heightPtr"></param>
	static void GetResolutionFromString(std::string resolution, uint16_t* widthPtr, uint16_t* heightPtr);
public:
	StartupOptions LoadStartupOptions();
	ResourcesLoader();
	~ResourcesLoader();
	int LoadResources(ResourceContainer* container);
};