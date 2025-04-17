#pragma once

#include <string>
#include "resource_container.hpp"

class ResourceAccessor
{
private:
	ResourceContainer* container;
public:
	ResourceAccessor(ResourceContainer* container);
	template <typename T>
	inline T* Get(const std::string& resourceName)
	{
		return container->GetResource<T>(resourceName);
	}
};