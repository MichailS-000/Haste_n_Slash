#include "resource_loader.hpp"

#include <stdexcept>
#include <fstream>
#include <sstream>
#include <format>

#include <rapidjson/document.h>

#include "../logger/logger.hpp"

ResourceLoader::ResourceLoader(ResourceContainer* container) : container(container)
{
}

void ResourceLoader::LoadResources(SDL_Renderer* renderer, ScriptsManager* scriptsManager)
{
	rapidjson::Document doc;
	std::ifstream resourceFile("resources/resources.json");

	if (!resourceFile.is_open())
	{
		Logger::LogError(1, "Missing file \"resources/resources.json\"");
		return;
	}

	std::stringstream stringStream;
	stringStream << resourceFile.rdbuf();
	resourceFile.close();

	if (doc.Parse(stringStream.str().c_str()).HasParseError())
	{
		Logger::LogError(1, std::format("Occured error while resource.json file parsing at: {}", (int)doc.GetErrorOffset()));
		return;
	}

	for (auto& obj : doc["Resources"].GetObject())
	{
		std::string objectName = obj.name.GetString();

		Logger::Log(std::format("Resource \"{}\" in loading...", objectName));

		if (objectName == "Startup Options")
		{
			StartupOptions* startupOptions = new StartupOptions();
			startupOptions->windowHeight = obj.value["window height"].GetInt();
			startupOptions->windowWidth = obj.value["window width"].GetInt();
			
			container->AddResource<StartupOptions>("Startup Options", startupOptions);
			
			continue;
		}
		else if (objectName == "Music Groups")
		{
			MusicGroups* groups = new MusicGroups();

			for (auto& musicGroup : obj.value.GetArray())
			{
				for (auto& [groupName, tracks] : musicGroup.GetObject())
				{
					for (auto& track : tracks.GetArray())
					{
						groups->AddMusicToGroup(groupName.GetString(), track.GetString());
					}
				}
			}

			container->AddResource<MusicGroups>("Music Groups", groups);
		
			continue;
		}

		if (obj.value["type"] == "image")
		{
			ImageResource image(obj.value, objectName);
			SDL_Texture* texture = image.Load(renderer);

			container->AddResource<SDL_Texture>(objectName, texture);
		}
		else if (obj.value["type"] == "music")
		{
			MusicResource music(obj.value, objectName);
			Mix_Music* track = music.Load();

			container->AddResource<Mix_Music>(objectName, track);
		}
		else if (obj.value["type"] == "sound")
		{
			SoundResource sound(obj.value, objectName);
			Mix_Chunk* SFX = sound.Load();

			container->AddResource<Mix_Chunk>(objectName, SFX);
		}
		else if (obj.value["type"] == "font")
		{
			FontResource font(obj.value, objectName);
			TTF_Font* fontInst = font.Load();

			container->AddResource<TTF_Font>(objectName, fontInst);
		}
		else if (obj.value["type"] == "script")
		{
			ScriptResource script(obj.value, objectName);
			CompiledScript* compiledScript = script.Load(scriptsManager);

			container->AddResource<CompiledScript>(objectName, compiledScript);
		}
	}
}
