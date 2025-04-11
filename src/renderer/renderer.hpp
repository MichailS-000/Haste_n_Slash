#pragma once

#include <entt/entt.hpp>

#include "../resources/resource_container.hpp"
#include "../components/camera.hpp"

class Renderer
{
private:
	std::map <std::string, SDL_Texture*> textures;
	SDL_Renderer* renderer;
	components::Camera* mainCamera;
	entt::registry* registry;
	ResourceContainer* container;
public:
	void UpdateRenderer();
	void LoadTextures();
	Renderer(SDL_Window* window, entt::registry* registry, ResourceContainer* container);
	~Renderer();
};