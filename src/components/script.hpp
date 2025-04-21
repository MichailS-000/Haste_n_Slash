#pragma once
#include "component.hpp"

namespace components
{
	struct Script
	{
		char initState = 0;
		std::string name = "null";
	};
}

template <>
const std::string ComponentTraits<components::Script>::name = "Sprite";
