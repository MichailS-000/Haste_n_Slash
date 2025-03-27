#pragma once
#include <SDL3/SDL.h>
#include <string>
#include <map>
#include <memory>
#include <entt/entt.hpp>
#include "../components/graphic.hpp"

class Renderer
{
private:
	std::map <std::string, SDL_Texture*> textures;
	SDL_Renderer* renderer;
public:
	void UpdateRenderer(const entt::registry& registry);
	Renderer(SDL_Window* window);
	~Renderer();
};