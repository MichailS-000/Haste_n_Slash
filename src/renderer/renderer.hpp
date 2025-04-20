#pragma once

#include <entt/entt.hpp>

#include "../components/components.hpp"

#include <SDL3/SDL.h>

class ResourceAccessor;

class Renderer
{
private:
	SDL_Renderer* renderer;
	entt::entity mainCamera;
	ResourceAccessor* resources;
	int screenWidth, screenHeight;
	SDL_FRect TransformToScreenRect(
		const components::Transform& transform, 
		const components::Transform& cameraTransform, 
		const components::Camera& camera, 
		float imageWidth, float imageHeight);
public:
	void UpdateRenderer(entt::registry* registry);
	SDL_Renderer* GetSDLRenderer();
	Renderer(SDL_Window* window, entt::registry* registry, ResourceAccessor* resources);
	~Renderer();
};