#include "renderer.hpp"

#include "../components/graphic.hpp"
#include "../components/generic.hpp"
#include "../components/text.hpp"
#include "../application/program_time.hpp"

#include <iostream>

void Renderer::UpdateRenderer()
{
	SDL_RenderClear(renderer);

	int screenWidth, screenHeight;
	SDL_GetWindowSize(SDL_GetRenderWindow(renderer), &screenWidth, &screenHeight);

	{
		auto view = registry->view<components::Background>();

		for (auto [entity, image] : view.each())
		{
			float imageWidth, imageHeight;
			SDL_GetTextureSize(textures[image.textureName], &imageWidth, &imageHeight);

			SDL_FRect sourceRect;
			sourceRect.h = imageHeight;
			sourceRect.w = imageWidth;
			sourceRect.x = 0;
			sourceRect.y = 0;

			SDL_FRect destinationRect;
			destinationRect.w = (screenHeight / imageHeight) * imageWidth;
			destinationRect.h = screenHeight;
			destinationRect.x = -(destinationRect.w - screenWidth) / 2;
			destinationRect.y = 0;

			SDL_RenderTexture(renderer, textures[image.textureName], &sourceRect, &destinationRect);
		}
	}

	{
		auto view = registry->view<components::Sprite, components::Position>();

		for (auto [entity, sprite, position] : view.each())
		{
			float imageWidth, imageHeight;
			SDL_GetTextureSize(textures[sprite.textureName], &imageWidth, &imageHeight);

			SDL_FRect sourceRect;
			sourceRect.h = imageHeight;
			sourceRect.w = imageWidth;
			sourceRect.x = 0;
			sourceRect.y = 0;

			SDL_FRect destinationRect;
			destinationRect.h = imageHeight * sprite.scaleY * mainCamera->scale;
			destinationRect.w = imageWidth * sprite.scaleX * mainCamera->scale;
			destinationRect.x = (position.positionX - mainCamera->posX) * mainCamera->scale - destinationRect.w / 2 + screenWidth / 2.f;
			destinationRect.y = (position.positionY - mainCamera->posY) * mainCamera->scale - destinationRect.h / 2 + screenHeight / 2.f;

			SDL_RenderTexture(renderer, textures[sprite.textureName], &sourceRect, &destinationRect);
		}
	}

	{
		auto view = registry->view<components::AnimatedSprite, components::Position>();

		for (auto [entity, sprite, position] : view.each())
		{
			float imageWidth, imageHeight;
			SDL_GetTextureSize(textures[sprite.textureName], &imageWidth, &imageHeight);

			int animationDuration = imageWidth / imageHeight;
			int frame = ((int)(Time::GetTime() / sprite.animationTempo)) % animationDuration;

			SDL_FRect sourceRect;
			sourceRect.h = imageHeight;
			sourceRect.w = imageWidth / animationDuration;
			sourceRect.x = (imageWidth / animationDuration) * frame;
			sourceRect.y = 0;

			SDL_FRect destinationRect;
			destinationRect.h = imageHeight * sprite.scaleY * mainCamera->scale;
			destinationRect.w = sourceRect.w * sprite.scaleX * mainCamera->scale;
			destinationRect.x = (position.positionX - mainCamera->posX) * mainCamera->scale - destinationRect.w / 2 + screenWidth / 2.f;
			destinationRect.y = (position.positionY - mainCamera->posY) * mainCamera->scale - destinationRect.h / 2 + screenHeight / 2.f;

			SDL_RenderTexture(renderer, textures[sprite.textureName], &sourceRect, &destinationRect);
		}
	}

	{
		auto view = registry->view<components::Position, components::Text>();

		for (auto [enitity, position, text] : view.each())
		{
			if (text.renderedText != nullptr)
			{
				SDL_DestroyTexture(text.renderedText);
			}

			SDL_Surface* renderedTextSurface = TTF_RenderText_Solid(container->GetFont(text.fontName), text.text.c_str(), text.text.length(), text.textColor);
			text.renderedText = SDL_CreateTextureFromSurface(renderer, renderedTextSurface);

			SDL_SetTextureScaleMode(text.renderedText, SDL_SCALEMODE_PIXELART);
			SDL_DestroySurface(renderedTextSurface);
			float imageWidth, imageHeight;
			SDL_GetTextureSize(text.renderedText, &imageWidth, &imageHeight);

			SDL_FRect sourceRect;
			sourceRect.h = imageHeight;
			sourceRect.w = imageWidth;
			sourceRect.x = 0;
			sourceRect.y = 0;

			float fontAspect = mainCamera->scale / imageHeight;

			SDL_FRect destinationRect;
			destinationRect.h = fontAspect * text.textScale * imageHeight;
			destinationRect.w = fontAspect * text.textScale * imageWidth;
			destinationRect.x = (position.positionX - mainCamera->posX) * mainCamera->scale - destinationRect.w / 2 + screenWidth / 2.f;
			destinationRect.y = (position.positionY - mainCamera->posY) * mainCamera->scale - destinationRect.h / 2 + screenHeight / 2.f;

			SDL_RenderTexture(renderer, text.renderedText, &sourceRect, &destinationRect);
		}
	}

	SDL_RenderPresent(renderer);
}

void Renderer::LoadTextures()
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

Renderer::Renderer(SDL_Window* window, entt::registry* registry, ResourceContainer* container) : registry(registry), container(container)
{
	renderer = SDL_CreateRenderer(window, NULL);

	auto entity = registry->create();
	registry->emplace<components::Camera>(entity);
	mainCamera = &registry->get<components::Camera>(entity);

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