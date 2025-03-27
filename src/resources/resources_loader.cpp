#include "resources_loader.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <format>
#include <rapidjson/document.h>

rapidjson::Document LoadDocument(const char* fileName)
{
	try
	{
		std::ifstream stream(fileName);
		if (!stream.is_open())
		{
			stream.close();
			throw std::exception();
		}

		std::stringstream sstream;
		sstream << stream.rdbuf();
		stream.close();

		rapidjson::Document resourcesDocument;

		if (resourcesDocument.Parse(sstream.str().c_str()).HasParseError())
		{
			//throw std::exception(std::format("Json document parse error {} at {}", resourcesDocument.GetParseError(), fileName).c_str());
		}

		return resourcesDocument;

	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

SDL_Surface* ResourcesLoader::loadSurface(const std::string& filename)
{
	std::string format = filename.substr(filename.find('.'), filename.size() - filename.find('.'));

	if (format == ".bmp")
	{
		return SDL_LoadBMP(filename.c_str());
	}
	else
	{
		throw std::exception(std::string("Image format dont supported: " + format).c_str());
	}

	return nullptr;
}

StartupOptions ResourcesLoader::LoadStartupOptions()
{
	rapidjson::Document resourcesDocument = LoadDocument("resources/options.json");

	StartupOptions options
	{
		resourcesDocument["options"].GetObject()["window width"].GetInt(),
		resourcesDocument["options"].GetObject()["window height"].GetInt()
	};

	return options;
}

ResourcesLoader::ResourcesLoader()
{ 
}

ResourcesLoader::~ResourcesLoader()
{

}

int ResourcesLoader::LoadResources(ResourceContainer* container)
{
	auto resourcesDocument = LoadDocument("resources/resources.json");

	for (auto& resource : resourcesDocument["Resources"].GetObject())
	{
		std::string resourceName = resource.name.GetString();
		
		if (resource.value["type"] == "image")
		{
			components::Image image;
			image.name = resourceName;
			image.surface = loadSurface(resource.value["source"].GetString());
			GetResolutionFromString(resource.value["resolution"].GetString(), &image.width, &image.height);
		
			container->AddImage(image);
		}
		else if (resource.value["type"] == "script")
		{
			UncompiledScript script;
			script.name = resourceName;
			script.sourcePath = resource.value["source"].GetString();

			auto permissionsArray = resource.value["permissions"].GetArray();

			for (auto& permission : permissionsArray)
			{
				script.permissions.push_back(permission.GetString());
			}

			container->AddUncompiledScript(script);
		}
	}

	return 0;
}