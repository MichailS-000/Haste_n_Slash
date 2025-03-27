#pragma once

#include <map>
#include <string>
#include <SDL3/SDL.h>
#include <entt/entt.hpp>
#include "resources/resource_container.hpp"
#include "input_manager/input_manager.hpp"
/// <summary>
/// Returns integer resolution by pointers from string
/// </summary>
/// <param name="resolution">String resolution</param>
/// <param name="widthPtr"></param>
/// <param name="heightPtr"></param>
extern void GetResolutionFromString(std::string resolution, uint16_t* widthPtr, uint16_t* heightPtr);

struct ExecutionEnviroment
{
	InputManager* input;
	entt::registry* applicationRegistry;
	ResourceContainer* resourcesContainer;
	entt::entity currentUpdatingEntity;
};

struct StartupOptions
{
	uint16_t windowWidth = 800;
	uint16_t windowHeight = 600;
};