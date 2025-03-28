#pragma once

#include "../renderer/renderer.hpp"
#include "../resources/resources_loader.hpp"
#include "../scripts/scripts_manager.hpp"

class Application
{
private:
	ExecutionEnviroment env;
	InputManager* inputManager;
	ScriptsManager* scriptsManager;
	ResourceContainer* resources;
	Renderer* renderer;
	ResourcesLoader* loader;
	entt::registry registry;
	SDL_Window* window;
public:
	Application();
	~Application();
	void Run();
};