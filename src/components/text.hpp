#pragma once
#include <string>
#include <SDL3/SDL.h>

namespace components
{
	struct Text : ComponentBase
	{
		std::string text = "";
		SDL_Color textColor = {255, 255, 255, 255};
		SDL_Texture* renderedText = nullptr;
		std::string fontName = "null";
		float textScale = 8;
	};
}