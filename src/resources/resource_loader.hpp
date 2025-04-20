#pragma once

#include "resource_types.hpp"
#include "resource_container.hpp"
#include "../scripts/scripts_manager.hpp"

class ResourceLoader
{
private:
	ResourceContainer* container;
public:
	ResourceLoader(ResourceContainer* container);
	void LoadResources(SDL_Renderer* renderer, ScriptsManager* scriptsManager);
};
