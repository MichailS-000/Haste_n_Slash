#include "renderer.hpp"

#include "../components/graphic.hpp"
#include "../components/generic.hpp"
#include "../components/text.hpp"
#include "../application/program_time.hpp"
#include "../resources/resource_accessor.hpp"

#include <SDL3_ttf/SDL_ttf.h>

#include <iostream>

void Renderer::UpdateRenderer(entt::registry* registry)
{
	SDL_RenderClear(renderer);

	int screenWidth, screenHeight;
	SDL_GetWindowSize(SDL_GetRenderWindow(renderer), &screenWidth, &screenHeight);

	auto [cameraPosition, camera] = registry->get<components::Transform, components::Camera>(mainCamera);

	{
		auto view = registry->view<components::Background>();

		for (auto [entity, image] : view.each())
		{
			float imageWidth, imageHeight;
			SDL_Texture* texture = resources->Get<SDL_Texture>(image.textureName);
			SDL_GetTextureSize(texture, &imageWidth, &imageHeight);

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

			SDL_RenderTexture(renderer, texture, &sourceRect, &destinationRect);
		}
	}

	{
		auto view = registry->view<components::Sprite, components::Transform>();

		for (auto [entity, sprite, transform] : view.each())
		{
			float imageWidth, imageHeight;
			SDL_Texture* texture = resources->Get<SDL_Texture>(sprite.textureName);
			SDL_GetTextureSize(texture, &imageWidth, &imageHeight);

			SDL_FRect sourceRect;
			sourceRect.h = imageHeight;
			sourceRect.w = imageWidth;
			sourceRect.x = 0;
			sourceRect.y = 0;

			SDL_FRect destinationRect;
			destinationRect.h = imageHeight * transform.scaleX * camera.scale;
			destinationRect.w = imageWidth * transform.scaleY * camera.scale;
			destinationRect.x = (transform.positionX - cameraPosition.positionX) * camera.scale - destinationRect.w / 2 + screenWidth / 2.f;
			destinationRect.y = (transform.positionY - cameraPosition.positionY) * camera.scale - destinationRect.h / 2 + screenHeight / 2.f;

			SDL_RenderTexture(renderer, texture, &sourceRect, &destinationRect);
		}
	}

	{
		auto view = registry->view<components::AnimatedSprite, components::Transform>();

		for (auto [entity, sprite, transform] : view.each())
		{
			float imageWidth, imageHeight;
			SDL_Texture* texture = resources->Get<SDL_Texture>(sprite.textureName);
			SDL_GetTextureSize(texture, &imageWidth, &imageHeight);

			int animationDuration = imageWidth / imageHeight;
			int frame = ((int)(Time::GetTime() / sprite.animationTempo)) % animationDuration;

			SDL_FRect sourceRect;
			sourceRect.h = imageHeight;
			sourceRect.w = imageWidth / animationDuration;
			sourceRect.x = (imageWidth / animationDuration) * frame;
			sourceRect.y = 0;

			SDL_FRect destinationRect;
			destinationRect.h = imageHeight * transform.scaleX * camera.scale;
			destinationRect.w = sourceRect.w * transform.scaleY * camera.scale;
			destinationRect.x = (transform.positionX - cameraPosition.positionX) * camera.scale - destinationRect.w / 2 + screenWidth / 2.f;
			destinationRect.y = (transform.positionY - cameraPosition.positionY) * camera.scale - destinationRect.h / 2 + screenHeight / 2.f;

			SDL_RenderTexture(renderer, texture, &sourceRect, &destinationRect);
		}
	}

	{
		auto view = registry->view<components::Transform, components::Text>();

		for (auto [enitity, transform, text] : view.each())
		{
			if (text.renderedText != nullptr)
			{
				SDL_DestroyTexture(text.renderedText);
			}

			SDL_Surface* renderedTextSurface = TTF_RenderText_Solid(resources->Get<TTF_Font>(text.fontName), text.text.c_str(), text.text.length(), text.textColor);
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

			float fontAspect = camera.scale / imageHeight;

			SDL_FRect destinationRect;
			destinationRect.h = fontAspect * text.textScale * imageHeight;
			destinationRect.w = fontAspect * text.textScale * imageWidth;
			destinationRect.x = (transform.positionX - cameraPosition.positionX) * camera.scale - destinationRect.w / 2 + screenWidth / 2.f;
			destinationRect.y = (transform.positionY - cameraPosition.positionY) * camera.scale - destinationRect.h / 2 + screenHeight / 2.f;

			SDL_RenderTexture(renderer, text.renderedText, &sourceRect, &destinationRect);
		}
	}

	SDL_RenderPresent(renderer);
}

SDL_Renderer* Renderer::GetSDLRenderer()
{
	return renderer;
}

Renderer::Renderer(SDL_Window* window, entt::registry* registry, ResourceAccessor* resources) : resources(resources)
{
	renderer = SDL_CreateRenderer(window, NULL);

	auto entity = registry->create();
	registry->emplace<components::Camera>(entity);
	registry->emplace<components::Transform>(entity);
	mainCamera = entity;

	if (renderer == NULL)
	{
		std::cout << SDL_GetError() << std::endl;
	}
}

Renderer::~Renderer()
{
}