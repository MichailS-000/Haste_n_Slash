#include "renderer.hpp"
#include <iostream>

void Renderer::UpdateRenderer(const entt::registry& registry)
{
	SDL_RenderClear(renderer);

	auto view = registry.view<components::Image>();

	for (auto [entity, image] : view.each())
	{
		if (!textures.contains(image.name))
		{
			textures[image.name] = SDL_CreateTextureFromSurface(renderer, image.surface);
		}
		
		switch (image.type)
		{
			case components::ImageType::FullScreen:
			{
				SDL_FRect srect;
				srect.x = 0;
				srect.y = 0;
				srect.h = image.height;
				srect.w = image.width;

				SDL_FRect drect;
				drect.x = 0;
				drect.y = 0;
				drect.h = 600;
				drect.w = 800;

				SDL_RenderTexture(renderer, textures[image.name], &srect, &drect);
			}
		break;
		default:
			break;
		}
	}

	SDL_RenderPresent(renderer);
}

void Renderer::LoadTextures(ResourceContainer* container)
{

}

Renderer::Renderer(SDL_Window* window)
{
	renderer = SDL_CreateRenderer(window, NULL);

	if (renderer == NULL)
	{
		std::cout << SDL_GetError() << std::endl;
	}
}

Renderer::~Renderer()
{
	for (auto& texture : textures)
	{
		SDL_DestroyTexture(texture.second);
	}

	SDL_DestroyRenderer(renderer);
}