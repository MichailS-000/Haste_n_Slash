#include "application.hpp"
#include "../logger/logger.hpp"


void GetResolutionFromString(std::string resolution, uint16_t* widthPtr, uint16_t* heightPtr)
{
	*widthPtr = atoi(resolution.substr(0, resolution.find('x')).c_str());
	*heightPtr = atoi(resolution.substr(resolution.find('x') + 1, 2).c_str());
}

Application::Application()
{
	Logger::Log("Application started!");

	inputManager = new InputManager();
	Logger::Log("Input manager created!");

	loader = new ResourcesLoader();
	Logger::Log("Resources loader created!");

	resources = new ResourceContainer();
	Logger::Log("Resources container created!");

	StartupOptions options = loader->LoadStartupOptions();
	Logger::Log("Startup options loaded!");

	window = SDL_CreateWindow("Haste & Slash", options.windowWidth, options.windowHeight, SDL_WINDOW_OPENGL);
	if (window)
	{
		Logger::Log("Window created!");
	}
	else
	{
		Logger::LogError(34, "Window creation error");
	}

	loader->LoadResources(resources);

	renderer = new Renderer(window);
	Logger::Log("Renderer created!");

	env = {}; 
	env.applicationRegistry = &registry;
	env.currentUpdatingEntity = entt::null;
	env.input = inputManager;
	env.resourcesContainer = resources;

	scriptsManager = new ScriptsManager(&env);
	Logger::Log("Scripts manager created!");
	scriptsManager->CompileScripts();

	components::Script script = scriptsManager->GetScriptInst("menuScript");
	entt::entity entity = registry.create();
	registry.emplace<components::Script>(entity, script);
}

Application::~Application()
{
	SDL_DestroyWindow(window);
	Logger::Log("Application terminated!");
}

void Application::Run()
{
	bool close = false;

	while (!close)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_EVENT_QUIT:
				close = true;
				break;
			}
		}

		inputManager->Update();
		scriptsManager->UpdateScripts();

		renderer->UpdateRenderer(registry);
	}
}
