#pragma once

#include <entt/entt.hpp>
#include "scripts_execution_enviroment.hpp"
#include "../renderer/renderer.hpp"

class AudioManager;
class ScriptsManager;
class ResourceContainer;

class Application
{
private:
	AudioManager* audio;
	ScriptsExecutionEnviroment* env;
	InputManager* inputManager;
	ScriptsManager* scriptsManager;
	ResourceContainer* resources;
	ResourceAccessor* resourceAccess;
	Renderer* renderer;
	entt::registry registry;
	SDL_Window* window;
public:
	Application();
	~Application();
	void Run();
};