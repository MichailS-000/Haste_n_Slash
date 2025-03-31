#pragma once

#include <entt/entt.hpp>
#include "../resources/resource_container.hpp"
#include "../input_manager/input_manager.hpp"
#include "../audio_manager/audio_manager.hpp"

struct ScriptsExecutionEnviroment
{
	InputManager* input;
	entt::registry* applicationRegistry;
	ResourceContainer* resourcesContainer;
	AudioManager* audio;
	entt::entity currentUpdatingEntity;
};