#pragma once
#include "component.hpp"

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
		float animationStartTime = 0;
		bool looped = false;
	};
}

template <>
const std::string ComponentTraits<components::Sprite>::name = "Sprite";

template <>
const std::string ComponentTraits<components::Background>::name = "Background";

template <>
const std::string ComponentTraits<components::AnimatedSprite>::name = "AnimatedSprite";
