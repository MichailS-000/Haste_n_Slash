#pragma once
#include "component.hpp"

namespace components
{
	struct Tag
	{
		std::string tag = "default";
	};

	struct Transform : ComponentBase
	{
		float positionX = 0;
		float positionY = 0;
		float scaleX = 1;
		float scaleY = 1;
	};

	struct RectTransform : ComponentBase
	{
		float pivotX = 0.5f;
		float pivotY = 0.5f;
		float alignX = 0;
		float alignY = 0;
		float height = 100;
		float width = 100;
	};
}

template <>
const std::string ComponentTraits<components::Transform>::name = "Transform";

template <>
const std::string ComponentTraits<components::RectTransform>::name = "RectTransform";

template <>
const std::string ComponentTraits<components::Tag>::name = "Tag";