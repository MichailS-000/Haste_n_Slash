#include "application.hpp"

#include "../logger/logger.hpp"
#include "../components/script.hpp"
#include "program_time.hpp"

Application::Application()
{
	SDL_Init(SDL_INIT_AUDIO);
	Mix_OpenAudio(0, NULL);

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

	renderer = new Renderer(window, &registry);
	Logger::Log("Renderer created!");

	renderer->LoadTextures(resources);
	Logger::Log("Images loaded into video memory");

	audio = new AudioManager(resources);

	env = {}; 
	env.applicationRegistry = &registry;
	env.currentUpdatingEntity = entt::null;
	env.input = inputManager;
	env.resourcesContainer = resources;

	scriptsManager = new ScriptsManager(&env);
	Logger::Log("Scripts manager created!");
	scriptsManager->CompileScripts();

	entt::entity entity = registry.create();
	components::Script menuScript;
	menuScript.name = "menuScript";
	registry.emplace<components::Script>(entity, menuScript);

	audio->StartPlayMusicGroup("background");
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
		Time::Update();

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
