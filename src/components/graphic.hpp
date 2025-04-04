#pragma once
#include <string>

namespace components
{
	struct Sprite
	{
		std::string textureName = "null";
		float scaleX = 1;
		float scaleY = 1;
	};

	struct Background
	{
		std::string textureName = "null";
	};

	struct AnimatedSprite
	{
		std::string textureName = "null";
		float scaleX = 1;
		float scaleY = 1;
		float animationTempo = 1;
	};
}
