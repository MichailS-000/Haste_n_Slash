#include "resources_loader.hpp"

#include "../logger/logger.hpp"
#include "../renderer/image.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <format>
#include <filesystem>
#include <rapidjson/document.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>

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
	if (!std::filesystem::exists(filename))
	{
		throw std::runtime_error(std::format("File {} does not exists", filename));
	}

	std::string format = filename.substr(filename.find('.'), filename.size() - filename.find('.'));
	SDL_Surface* img;

	if (format == ".bmp")
	{
		img = SDL_LoadBMP(filename.c_str());
	}
	else if (format == ".png")
	{
		img = IMG_Load(filename.c_str());
	}
	else if (format == ".jpeg")
	{
		img = IMG_Load(filename.c_str());
	}
	else
	{
		throw std::runtime_error(std::string("Image format dont supported: " + format).c_str());
	}

	if (img == NULL)
	{
		throw std::runtime_error(std::format("Image loading error: {}", filename));
	}

	return img;
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
				Image image;
				image.name = resourceName;
				image.surface = loadSurface(resource.value["source"].GetString());

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
			else if (resource.value["type"] == "music")
			{
				Mix_Music* music = Mix_LoadMUS(resource.value["source"].GetString());
				if (music == NULL)
				{
					Logger::LogError(1, std::format("\"{}\" decoding error: {}", resourceName, SDL_GetError()));
					continue;
				}

				std::string group = resource.value["group"].GetString();
				container->AddMusic(music, group);
			}
			else if (resource.value["type"] == "sound")
			{
				Mix_Chunk* sound = Mix_LoadWAV(resource.value["source"].GetString());
				if (sound == NULL)
				{
					Logger::LogError(1, std::format("\"{}\" decoding error: {}", resourceName, SDL_GetError()));
					continue;
				}

				container->AddSound(sound, resourceName);
			}
		}
	}
	catch (const std::runtime_error& e)
	{
		Logger::LogError(1, e.what());
	}

	return 0;
}