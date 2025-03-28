#pragma once

#include <map>
#include <memory>
#include <queue>

#include "../renderer/image.hpp"
#include "../scripts/uncompiled_script.hpp"

class ResourceContainer
{
private:
	std::queue<UncompiledScript*> uncompiledScripts;
	std::queue<Image*> images;
public:
	void AddImage(Image& img);
	void AddUncompiledScript(UncompiledScript& script);

	Image* GetNextImage();
	UncompiledScript* GetNextUncompiledScript();

	ResourceContainer();
	~ResourceContainer();
};

