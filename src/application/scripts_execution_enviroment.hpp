#pragma once

#include <entt/entt.hpp>
#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>

class InputManager;
class ResourceAccessor;
class AudioManager;

struct ScriptsExecutionEnviroment
{
	ScriptsExecutionEnviroment(entt::registry* registry,
								InputManager* inputManager, 
								ResourceAccessor* resources, 
								AudioManager* audio) : 
		applicationRegistry(registry),
		input(inputManager),
		resources(resources),
		audio(audio),
		currentUpdatingEntity(entt::entity{})
	{
	}
	InputManager* input;
	entt::registry* applicationRegistry;
	ResourceAccessor* resources;
	AudioManager* audio;
	entt::entity currentUpdatingEntity;
};