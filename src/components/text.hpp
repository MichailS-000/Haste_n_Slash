#pragma once
#include <string>
#include <SDL3/SDL.h>

namespace components
{
	struct Text : ComponentBase
	{
		SDL_Color textColor = {255, 255, 255, 255};
		SDL_Texture* renderedText = nullptr;
		std::string fontName = "null";
		float textScale = 8;

		bool needRendering = true;

		void SetText(std::string& text)
		{
			this->text = text;
			needRendering = true;
		}

		void SetText(std::string&& text)
		{
			this->text = text;
			needRendering = true;
		}

		std::string& getString()
		{
			return text;
		}

	private:
		std::string text = "";
	};
}