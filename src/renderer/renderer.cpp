#include "renderer.hpp"

#include "../components/graphic.hpp"

#include <iostream>

void Renderer::UpdateRenderer(const entt::registry& registry)
{
	SDL_RenderClear(renderer);

	auto view = registry.view<components::Background>();

	for (auto [entity, image] : view.each())
	{
		int screenWidth, screenHeight;
		SDL_GetWindowSize(SDL_GetRenderWindow(renderer), &screenWidth, &screenHeight);
		float imageWidth, imageHeight;
		SDL_GetTextureSize(textures[image.textureName], &imageWidth, &imageHeight);

		SDL_FRect sourceRect;
		sourceRect.h = imageHeight;
		sourceRect.w = imageWidth;
		sourceRect.x = 0;
		sourceRect.y = 0;

		SDL_FRect destinationRect;
		destinationRect.w =	(screenHeight / imageHeight) * imageWidth;
		destinationRect.h = screenHeight;
		destinationRect.x = -(destinationRect.w - screenWidth) / 2;
		destinationRect.y = 0;

		SDL_RenderTexture(renderer, textures[image.textureName], &sourceRect, &destinationRect);
	}

	SDL_RenderPresent(renderer);
}

void Renderer::LoadTextures(ResourceContainer* container)
{
	Image* image;
	while ((image = container->GetNextImage()) != nullptr)
	{
		textures.emplace(image->name, SDL_CreateTextureFromSurface(renderer, image->surface));
		SDL_SetTextureScaleMode(textures[image->name], SDL_SCALEMODE_PIXELART);

		SDL_DestroySurface(image->surface);
		delete image;
	}
}

Renderer::Renderer(SDL_Window* window, entt::registry* registry)
{
	renderer = SDL_CreateRenderer(window, NULL);

	auto entity = registry->create();
	registry->emplace<components::Camera>(entity);
	mainCamera = entity;

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