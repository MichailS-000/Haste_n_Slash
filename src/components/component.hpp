#pragma once

struct ComponentBase
{
	bool enabled;
};

template<typename T>
struct ComponentTraits
{
	static const std::string name;
};