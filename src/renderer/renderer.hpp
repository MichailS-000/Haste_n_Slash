#pragma once

#include <entt/entt.hpp>

#include "../components/camera.hpp"

#include <SDL3/SDL.h>

class ResourceAccessor;

class Renderer
{
private:
	SDL_Renderer* renderer;
	entt::entity mainCamera;
	ResourceAccessor* resources;
public:
	void UpdateRenderer(entt::registry* registry);
	SDL_Renderer* GetSDLRenderer();
	Renderer(SDL_Window* window, entt::registry* registry, ResourceAccessor* resources);
	~Renderer();
};