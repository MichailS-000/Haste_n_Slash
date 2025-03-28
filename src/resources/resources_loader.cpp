#include "resources_loader.hpp"

#include "../logger/logger.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <format>
#include <rapidjson/document.h>

rapidjson::Document LoadDocument(const char* fileName)
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
		throw std::runtime_error(std::format("Json document parse error {} at {}", fileName, resourcesDocument.GetErrorOffset()).c_str());
	}

	return resourcesDocument;
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
		throw std::runtime_error(std::string("Image format dont supported: " + format).c_str());
	}

	return nullptr;
}

void ResourcesLoader::GetResolutionFromString(std::string resolution, uint16_t* widthPtr, uint16_t* heightPtr)
{
	*widthPtr = atoi(resolution.substr(0, resolution.find('x')).c_str());
	*heightPtr = atoi(resolution.substr(resolution.find('x') + 1, 2).c_str());
}

StartupOptions ResourcesLoader::LoadStartupOptions()
{
	rapidjson::Document resourcesDocument;

	try
	{
		resourcesDocument = LoadDocument("resources/options.json");
	}
	catch (const std::runtime_error& e)
	{
		Logger::LogError(1, e.what());
	}

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
	try
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
	}
	catch (const std::runtime_error& e)
	{
		Logger::LogError(1, e.what());
	}

	return 0;
}