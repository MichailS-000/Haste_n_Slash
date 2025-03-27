#pragma once
#include <SDL3/SDL.h>
#include <entt/entt.hpp>

#include "../renderer/renderer.hpp"
#include "../resources/resources_loader.hpp"
#include "../scripts/scripts_manager.hpp"
#include "../haste_n_slash.hpp"

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