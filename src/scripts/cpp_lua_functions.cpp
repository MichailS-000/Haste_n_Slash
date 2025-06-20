#include "cpp_lua_functions.hpp"

#include "../components/components.hpp"
#include "../application/program_time.hpp"
#include "../input_manager/input_manager.hpp"
#include "../audio_manager/audio_manager.hpp"

#include <LuaBridge/LuaBridge.h>
#include <lua.hpp>

void LinkGenericLib(lua_State* state, ScriptsExecutionEnviroment* env)
{
	luabridge::getGlobalNamespace(state)
		.beginNamespace("time")
		.addFunction("getTime", []() 
			{
				return Time::GetTime();
			})
		.addFunction("getDeltaTime", []()
			{
				return Time::GetDeltaTime();
			})
		.endNamespace();
}

void LinkEntityLib(lua_State* state, ScriptsExecutionEnviroment* env)
{
	luabridge::getGlobalNamespace(state)
		.beginNamespace("entity")
		.addFunction("getCurrentEntity", [env = env] 
			{
				return (int)env->currentUpdatingEntity;
			})
		.addFunction("addEntity", [env = env]() 
			{
				return (int)env->applicationRegistry->create();
			})
		.addFunction("addScriptToEntity", [env = env](std::string scriptName, int entity) 
			{
				components::Script script;
				script.name = scriptName;
				env->applicationRegistry->emplace<components::Script>((entt::entity)entity, script);
			})
		.addFunction("destroyEntity", [env = env](int entity) 
			{
				env->applicationRegistry->destroy((entt::entity)entity);
			})
		.addFunction("movePosition", [env = env](int entity, float deltaX, float deltaY) 
			{
				components::Transform& transform = env->applicationRegistry->get<components::Transform>((entt::entity)entity);
				transform.positionX += deltaX;
				transform.positionY += deltaY;
			})
		.addFunction("setPosition", [env = env](int entity, float x, float y) 
			{
				components::Transform& transform = env->applicationRegistry->get<components::Transform>((entt::entity)entity);
				transform.positionX = x;
				transform.positionY = y;
			})
		.endNamespace();
}

void LinkInputLib(lua_State* state, ScriptsExecutionEnviroment* env)
{
	luabridge::getGlobalNamespace(state)
		.beginNamespace("input")
		.addFunction("getKeyDown", [env = env](std::string key)
			{
				return env->input->GetKeyDown(std::move(key));
			})
		.addFunction("getKey", [env = env](std::string key)
			{
				return env->input->GetKey(std::move(key));
			})
		.addFunction("ketKeyUp", [env = env](std::string key)
			{
				return env->input->GetKeyUp(std::move(key));
			})
		.endNamespace();
}

void LinkGraphicsLib(lua_State* state, ScriptsExecutionEnviroment* env)
{
	luabridge::getGlobalNamespace(state)
		.beginNamespace("graphics")
		.addFunction("addBackground", [env = env](std::string textureName) 
			{
				entt::entity entity = env->applicationRegistry->create();
				env->applicationRegistry->emplace<components::Background>(entity, textureName);
			})
		.endNamespace();
}

void LinkAudioLib(lua_State* state, ScriptsExecutionEnviroment* env)
{
	luabridge::getGlobalNamespace(state)
		.beginNamespace("audio")
		.addFunction("playSound", [audio = env->audio](std::string soundName)
			{
				audio->PlaySoundOneShot(soundName);
			})
		.addFunction("startPlayMusicGroup", [audio = env->audio](std::string musicGroupName)
			{
				audio->StartPlayMusicGroup(musicGroupName);
			})
		.addFunction("stopPlayingMusic", [audio = env->audio]()
			{
				audio->StopPlayingMusic();
			})
		.endNamespace();
}
