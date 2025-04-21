#include "renderer.hpp"

#include "../components/graphic.hpp"
#include "../components/generic.hpp"
#include "../components/text.hpp"
#include "../application/program_time.hpp"
#include "../resources/resource_accessor.hpp"

#include <SDL3_ttf/SDL_ttf.h>

#include <iostream>

SDL_FRect Renderer::TransformToScreenRect(const components::Transform& transform, const components::Transform& cameraTransform, const components::Camera& camera, float imageWidth, float imageHeight)
{
	float aspect = imageWidth / imageHeight;

	SDL_FRect destinationRect;
	destinationRect.h = transform.scaleY * camera.scale;
	destinationRect.w = aspect * transform.scaleX * camera.scale;
	destinationRect.x = (transform.positionX - cameraTransform.positionX) * camera.scale - destinationRect.w / 2 + screenWidth / 2.f;
	destinationRect.y = -(transform.positionY - cameraTransform.positionY) * camera.scale - destinationRect.h / 2 + screenHeight / 2.f;

	return destinationRect;
}

void Renderer::UpdateRenderer(entt::registry* registry)
{
	SDL_RenderClear(renderer);

	SDL_GetWindowSize(SDL_GetRenderWindow(renderer), &screenWidth, &screenHeight);

	auto [cameraTransform, camera] = registry->get<components::Transform, components::Camera>(mainCamera);

	{
		auto view = registry->view<components::Background>();

		for (auto [entity, image] : view.each())
		{
			if (!image.enabled)
				continue;

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
			if (!sprite.enabled)
				continue;

			float imageWidth, imageHeight;
			SDL_Texture* texture = resources->Get<SDL_Texture>(sprite.textureName);
			SDL_GetTextureSize(texture, &imageWidth, &imageHeight);

			SDL_FRect sourceRect;
			sourceRect.h = imageHeight;
			sourceRect.w = imageWidth;
			sourceRect.x = 0;
			sourceRect.y = 0;

			SDL_FRect destinationRect = TransformToScreenRect(transform, cameraTransform, camera, imageWidth, imageHeight);

			SDL_RenderTexture(renderer, texture, &sourceRect, &destinationRect);
		}
	}

	{
		auto view = registry->view<components::AnimatedSprite, components::Transform>();

		for (auto [entity, sprite, transform] : view.each())
		{
			if (!sprite.enabled)
				continue;

			float imageWidth, imageHeight;
			SDL_Texture* texture = resources->Get<SDL_Texture>(sprite.textureName);
			SDL_GetTextureSize(texture, &imageWidth, &imageHeight);

			int animationDuration = imageWidth / imageHeight;
			
			int frame = 0;

			if (sprite.looped)
			{
				frame = ((int)((Time::GetTime() + sprite.animationStartTime) / sprite.animationTempo)) % animationDuration;
			}
			else
			{
				frame = ((int)((Time::GetTime() + sprite.animationStartTime) / sprite.animationTempo));
				frame = SDL_clamp(frame, 0, animationDuration - 1);
			}

			SDL_FRect sourceRect;
			sourceRect.h = imageHeight;
			sourceRect.w = imageWidth / animationDuration;
			sourceRect.x = (imageWidth / animationDuration) * frame;
			sourceRect.y = 0;

			SDL_FRect destinationRect = TransformToScreenRect(transform, cameraTransform, camera, imageWidth / animationDuration, imageHeight);

			SDL_RenderTexture(renderer, texture, &sourceRect, &destinationRect);
		}
	}

	{
		auto view = registry->view<components::Transform, components::Text>();

		for (auto [enitity, transform, text] : view.each())
		{
			if (!text.enabled)
				continue;

			if (text.needRendering)
			{
				if (text.renderedText != nullptr)
				{
					SDL_DestroyTexture(text.renderedText);
				}

				SDL_Surface* renderedTextSurface = TTF_RenderText_Solid(resources->Get<TTF_Font>(text.fontName), text.getString().c_str(), text.getString().length(), text.textColor);
				text.renderedText = SDL_CreateTextureFromSurface(renderer, renderedTextSurface);

				SDL_SetTextureScaleMode(text.renderedText, SDL_SCALEMODE_PIXELART);
				SDL_DestroySurface(renderedTextSurface);

				text.needRendering = false;
			}

			float imageWidth, imageHeight;
			SDL_GetTextureSize(text.renderedText, &imageWidth, &imageHeight);

			SDL_FRect sourceRect;
			sourceRect.h = imageHeight;
			sourceRect.w = imageWidth;
			sourceRect.x = 0;
			sourceRect.y = 0;

			SDL_FRect destinationRect = TransformToScreenRect(transform, cameraTransform, camera, imageWidth, imageHeight);

			SDL_RenderTexture(renderer, text.renderedText, &sourceRect, &destinationRect);
		}
	}

	SDL_RenderPresent(renderer);
}

SDL_Renderer* Renderer::GetSDLRenderer()
{
	return renderer;
}

Renderer::Renderer(SDL_Window* window, entt::registry* registry, ResourceAccessor* resources) : resources(resources), screenHeight(600), screenWidth(800)
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