#pragma once
#include <string>

namespace components
{
	struct Sprite
	{
		std::string textureName = "null";
	};

	struct Background
	{
		std::string textureName = "null";
	};

	struct AnimatedSprite
	{
		std::string textureName = "null";
		float animationTempo = 1;
	};
}
