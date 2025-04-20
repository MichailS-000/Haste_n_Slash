#pragma once
#include <rapidjson/document.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>

#include <string>
#include <vector>

class CompiledScript;
class ScriptsManager;

class IResource
{
public:
	IResource(const rapidjson::Value& jsonObj, const std::string& name);
	virtual ~IResource() = default;
	virtual const std::string& GetName() { return name; }
	virtual const std::string& GetType() { return type; }
private:
	std::string name = "null";
	std::string type = "null";
};

class ImageResource : public IResource
{
public:
	ImageResource(const rapidjson::Value& jsonObj, const std::string& name);
	SDL_Texture* Load(SDL_Renderer* renderer);
private:
	std::string source;
};

class MusicResource : public IResource
{
public:
	MusicResource(const rapidjson::Value& jsonObj, const std::string& name);
	Mix_Music* Load();
	const std::string& GetGroup();
private:
	std::string group;
	std::string source;
};

class SoundResource : public IResource
{
public:
	SoundResource(const rapidjson::Value& jsonObj, const std::string& name);
	Mix_Chunk* Load();
private:
	std::string source;
};

class FontResource : public IResource
{
public:
	FontResource(const rapidjson::Value& jsonObj, const std::string& name);
	TTF_Font* Load();
private:
	std::string source;
	int size;
};

class ScriptResource : public IResource
{
public:
	ScriptResource(const rapidjson::Value& jsonObj, const std::string& name);
	CompiledScript* Load(ScriptsManager* scriptManager);
private:
	std::string source;
	std::vector<std::string> permissions;
};