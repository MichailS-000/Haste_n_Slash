#include "application.hpp"

#include "../components/script.hpp"
#include "../components/generic.hpp"
#include "../components/graphic.hpp"
#include "../components/text.hpp"

#include "../logger/logger.hpp"

#include "program_time.hpp"

#include <SDL3_mixer/SDL_mixer.h>

#include "../input_manager/input_manager.hpp"
#include "../resources/resource_container.hpp"
#include "../resources/resource_loader.hpp"
#include "../resources/resource_accessor.hpp"
#include "../audio_manager/audio_manager.hpp"

Application::Application()
{
	SDL_Init(SDL_INIT_AUDIO);
	Mix_OpenAudio(0, NULL);

	Logger::Log("Application started!");

	inputManager = new InputManager();
	Logger::Log("Input manager created!");

	resources = new ResourceContainer();
	Logger::Log("Resources container created!");

	ResourceLoader loader(resources);

	resourceAccess = new ResourceAccessor(resources);

	window = SDL_CreateWindow("Haste & Slash", 800, 600, SDL_WINDOW_OPENGL);
	if (window)
	{
		Logger::Log("Window created!");
	}
	else
	{
		Logger::LogError(34, "Window creation error");
	}

	renderer = new Renderer(window, &registry, resourceAccess);
	Logger::Log("Renderer created!");

	audio = new AudioManager(resourceAccess);
	Logger::Log("Audio manager created!");

	env = {}; 
	env.applicationRegistry = &registry;
	env.currentUpdatingEntity = entt::null;
	env.input = inputManager;
	env.resources = resourceAccess;
	env.audio = audio;

	scriptsManager = new ScriptsManager(&env);

	loader.LoadResources(renderer->GetSDLRenderer(), scriptsManager);

	StartupOptions* options = resourceAccess->Get<StartupOptions>("Startup Options");
	SDL_SetWindowSize(window, options->windowWidth, options->windowHeight);

	entt::entity entity = registry.create();
	components::Script menuScript;
	menuScript.name = "menuScript";
	registry.emplace<components::Script>(entity, menuScript);

	entt::entity sprite = registry.create();
	components::Sprite cSprite = {};
	cSprite.textureName = "frame";
	registry.emplace<components::Sprite>(entity, cSprite);
	registry.emplace<components::Position>(entity);

	entt::entity animatedSprite = registry.create();
	components::AnimatedSprite aSprite = {};
	aSprite.textureName = "platAnim";
	aSprite.animationTempo = 0.2f;
	registry.emplace<components::AnimatedSprite>(animatedSprite, aSprite);
	registry.emplace<components::Position>(animatedSprite);

	entt::entity text = registry.create();
	components::Text cText;
	cText.fontName = "monocraft";
	cText.text = "Hello FONTS!!";
	registry.emplace<components::Text>(text, cText);
	registry.emplace<components::Position>(text);
}

Application::~Application()
{
	delete inputManager;
	delete resources;
	delete scriptsManager;
	delete audio;
	delete renderer;

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

		renderer->UpdateRenderer(&registry);
	}
}
