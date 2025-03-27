#pragma once

#include <string>
#include <map>
#include <memory>
#include <queue>

#include "../components/graphic.hpp"
#include "../scripts/uncompiled_script.hpp"


template <typename T>
using TypeContainer = std::map<std::string, std::unique_ptr<T>>;

class ResourceContainer
{
private:
	std::queue<UncompiledScript*> uncompiledScripts;
	TypeContainer<components::Image> images;
	void FreeImages();
public:
	void AddImage(components::Image& img);
	void AddUncompiledScript(UncompiledScript& script);

	components::Image GetImageInst(std::string name);
	UncompiledScript* GetNextUncompiledScript();

	ResourceContainer();
	~ResourceContainer();
};

