#pragma once
#include "component.hpp"

namespace components
{
	struct Camera
	{
		float scale = 100.f;
	};
}

template <>
const std::string ComponentTraits<components::Camera>::name = "Camera";