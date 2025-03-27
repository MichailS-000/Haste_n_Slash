#pragma once
#include <SDL3/SDL.h>
#include <string>

namespace components
{
	enum ImageType
	{
		FullScreen,
		PositionOrented,
		UI
	};

	struct Image
	{
		std::string name;
		ImageType type = ImageType::FullScreen;
		SDL_Surface* surface = nullptr;
		uint16_t width = 100;
		uint16_t height = 100;
	};
}
