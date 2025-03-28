#include "resource_container.hpp"
#include <format>

#include "../logger/logger.hpp"

void ResourceContainer::AddImage(Image& img)
{
	images.push(new Image(img));
	Logger::Log(std::format("Image \"{}\" loaded", img.name));
}

void ResourceContainer::AddUncompiledScript(UncompiledScript& script)
{
	uncompiledScripts.push(new UncompiledScript(script));
	Logger::Log(std::format("Script \"{}\" loaded", script.name));
}

Image* ResourceContainer::GetNextImage()
{
	if (images.size() > 0)
	{
		Image* img = new Image(*images.front());
		delete images.front();
		images.pop();
		return img;
	}
	else
	{
		return nullptr;
	}
}

UncompiledScript* ResourceContainer::GetNextUncompiledScript()
{
	if (uncompiledScripts.size() > 0)
	{
		UncompiledScript* script = new UncompiledScript(*uncompiledScripts.front());
		delete uncompiledScripts.front();
		uncompiledScripts.pop();
		return script;
	}
	else
	{
		return nullptr;
	}
}

ResourceContainer::ResourceContainer()
{
}

ResourceContainer::~ResourceContainer()
{
}
