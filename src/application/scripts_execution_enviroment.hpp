#pragma once

#include <entt/entt.hpp>

class InputManager;
class ResourceAccessor;
class AudioManager;

struct ScriptsExecutionEnviroment
{
	InputManager* input;
	entt::registry* applicationRegistry;
	ResourceAccessor* resources;
	AudioManager* audio;
	entt::entity currentUpdatingEntity;
};