#pragma once

#include <entt/entt.hpp>
#include "../resources/resource_container.hpp"
#include "../input_manager/input_manager.hpp"

struct ScriptsExecutionEnviroment
{
	InputManager* input;
	entt::registry* applicationRegistry;
	ResourceContainer* resourcesContainer;
	entt::entity currentUpdatingEntity;
};