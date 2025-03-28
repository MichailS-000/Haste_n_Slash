#include "resource_container.hpp"
#include <format>

#include "../logger/logger.hpp"

void ResourceContainer::FreeImages()
{
	for (auto& [imgName, img] : images)
	{
		SDL_DestroySurface(img.get()->surface);
		img.reset();
	}
}

void ResourceContainer::AddImage(components::Image& img)
{
	try
	{
		if (!images.contains(img.name))
			images[img.name] = std::make_unique<components::Image>(img);
		else
			throw std::runtime_error(std::format("Trying to add image that have indentical name ({})", img.name).c_str());
	}
	catch (const std::runtime_error& e)
	{
		Logger::LogError(2, e.what());
	}

	Logger::Log(std::format("Image {} loaded", img.name));
}

void ResourceContainer::AddUncompiledScript(UncompiledScript& script)
{
	uncompiledScripts.push(new UncompiledScript(script));
	Logger::Log(std::format("Script {} loaded", script.name));
}

components::Image ResourceContainer::GetImageInst(std::string name)
{
	try
	{
		if (images.contains(name))
			return components::Image(*images[name]);
		else
			throw std::runtime_error(std::format("Trying to get non existing image ({})", name).c_str());
	}
	catch (const std::runtime_error& e)
	{
		Logger::LogError(2, e.what());
		return components::Image();
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
	FreeImages();
}
