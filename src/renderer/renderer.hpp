#pragma once

#include <string>
#include <map>
#include <memory>
#include <entt/entt.hpp>
#include "../resources/resource_container.hpp"
class Renderer
{
private:
	std::map <std::string, SDL_Texture*> textures;
	SDL_Renderer* renderer;
public:
	void UpdateRenderer(const entt::registry& registry);
	void LoadTextures(ResourceContainer* container);
	Renderer(SDL_Window* window);
	~Renderer();
};