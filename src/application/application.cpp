#include "application.hpp"

void GetResolutionFromString(std::string resolution, uint16_t* widthPtr, uint16_t* heightPtr)
{
	*widthPtr = atoi(resolution.substr(0, resolution.find('x')).c_str());
	*heightPtr = atoi(resolution.substr(resolution.find('x') + 1, 2).c_str());
}

Application::Application()
{
	inputManager = new InputManager();
	loader = new ResourcesLoader();
	resources = new ResourceContainer();

	StartupOptions options = loader->LoadStartupOptions();

	window = SDL_CreateWindow("Haste & Slash", options.windowWidth, options.windowHeight, SDL_WINDOW_OPENGL);

	loader->LoadResources(resources);

	renderer = new Renderer(window);

	env = {}; 
	env.applicationRegistry = &registry;
	env.currentUpdatingEntity = entt::null;
	env.input = inputManager;
	env.resourcesContainer = resources;

	scriptsManager = new ScriptsManager(&env);
	scriptsManager->CompileScripts();

	components::Script script = scriptsManager->GetScriptInst("menuScript");
	auto entity = registry.create();
	registry.emplace<components::Script>(entity, script);
}

Application::~Application()
{
	SDL_DestroyWindow(window);
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
