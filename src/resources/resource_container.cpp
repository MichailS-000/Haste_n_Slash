#include "resource_container.hpp"
#include <format>

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
	if (!images.contains(img.name))
		images[img.name] = std::make_unique<components::Image>(img);
	else
		throw std::exception(std::format("Trying to add image that have indentical name ({})", img.name).c_str());
}

void ResourceContainer::AddUncompiledScript(UncompiledScript& script)
{
	uncompiledScripts.push(new UncompiledScript(script));
}

components::Image ResourceContainer::GetImageInst(std::string name)
{
	if (images.contains(name))
		return components::Image(*images[name]);
	else
		throw std::exception(std::format("Trying to get non existing image ({})", name).c_str());
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
