#pragma once

namespace components
{
	struct ComponentBase
	{
		bool enabled = true;
	};
}

template<typename T>
struct ComponentTraits
{
	static const std::string name;
};