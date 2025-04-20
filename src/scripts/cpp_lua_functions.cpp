#include "cpp_lua_functions.hpp"

#include "../components/components.hpp"
#include "../application/program_time.hpp"
#include "../input_manager/input_manager.hpp"
#include "../audio_manager/audio_manager.hpp"
#include "../logger/logger.hpp"

#include <LuaBridge/LuaBridge.h>
#include <lua.hpp>

template <typename T>
bool HasComponent(const entt::registry& registry, const entt::entity& entity)
{
	return (registry.try_get<T>(entity) != nullptr);
}

bool ValidateEntity(const entt::registry& registry, const entt::entity& entity)
{
	return registry.valid(entity);
}

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
		.addFunction("selfDestruct", [env = env]() 
			{
				env->applicationRegistry->destroy(env->currentUpdatingEntity);
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
				if (!ValidateEntity(*env->applicationRegistry, (entt::entity)entity))
				{
					throw std::runtime_error("Trying to handle invalid entity");
				}

				components::Script script;
				script.name = scriptName;

				if (HasComponent<components::Script>(*env->applicationRegistry, (entt::entity)entity))
				{
					throw std::runtime_error("Entity already contains Script component");
				}
				else
				{
					env->applicationRegistry->emplace<components::Script>((entt::entity)entity, script);
				}
			})
		.addFunction("destroyEntity", [env = env](int entity) 
			{
				env->applicationRegistry->destroy((entt::entity)entity);
			})
		.addFunction("movePosition", [env = env](int entity, float deltaX, float deltaY) 
			{
				if (!ValidateEntity(*env->applicationRegistry, (entt::entity)entity))
				{
					throw std::runtime_error("Trying to handle invalid entity");
				}

				if (!HasComponent<components::Transform>(*env->applicationRegistry, (entt::entity)entity))
				{
					throw std::runtime_error("Entity does not contains Transform component");
				}

				components::Transform& transform = env->applicationRegistry->get<components::Transform>((entt::entity)entity);
				transform.positionX += deltaX;
				transform.positionY += deltaY;
			})
		.addFunction("setPosition", [env = env](int entity, float x, float y) 
			{
				if (!ValidateEntity(*env->applicationRegistry, (entt::entity)entity))
				{
					throw std::runtime_error("Trying to handle invalid entity");
				}

				if (!HasComponent<components::Transform>(*env->applicationRegistry, (entt::entity)entity))
				{
					throw std::runtime_error("Entity does not contains Transform component");
				}

				components::Transform& transform = env->applicationRegistry->get<components::Transform>((entt::entity)entity);
				transform.positionX = x;
				transform.positionY = y;
			})
		.addFunction("setScale", [env = env](int entity, float x, float y) 
			{
				if (!ValidateEntity(*env->applicationRegistry, (entt::entity)entity))
				{
					throw std::runtime_error("Trying to handle invalid entity");
				}

				if (!HasComponent<components::Transform>(*env->applicationRegistry, (entt::entity)entity))
				{
					throw std::runtime_error("Entity does not contains Transform component");
				}

				components::Transform& transform = env->applicationRegistry->get<components::Transform>((entt::entity)entity);
				transform.scaleX = x;
				transform.scaleY = y;
			})
		.addFunction("addTransform", [env = env](int entity)
			{
				if (!ValidateEntity(*env->applicationRegistry, (entt::entity)entity))
				{
					throw std::runtime_error("Trying to handle invalid entity");
				}

				if (HasComponent<components::Transform>(*env->applicationRegistry, (entt::entity)entity))
				{
					throw std::runtime_error("Entity already contains Transform component");
				}
				
				env->applicationRegistry->emplace<components::Transform>((entt::entity)entity);
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
		.addFunction("addBackground", [env = env](std::string textureName, int entity) 
			{
				if (!ValidateEntity(*env->applicationRegistry, (entt::entity)entity))
				{
					throw std::runtime_error("Trying to handle invalid entity");
				}

				if (HasComponent<components::Background>(*env->applicationRegistry, (entt::entity)entity))
				{
					throw std::runtime_error("Entity already contains Background component");
				}

				components::Background bg;
				bg.textureName = textureName;
				env->applicationRegistry->emplace<components::Background>((entt::entity)entity, bg);
			})
		.addFunction("addSprite", [env = env](std::string textureName, int entity)
			{
				if (!ValidateEntity(*env->applicationRegistry, (entt::entity)entity))
				{
					throw std::runtime_error("Trying to handle invalid entity");
				}

				if (HasComponent<components::Sprite>(*env->applicationRegistry, (entt::entity)entity))
				{
					throw std::runtime_error("Entity already contains Sprite component");
				}

				components::Sprite sprite;
				sprite.textureName = textureName;
				env->applicationRegistry->emplace<components::Sprite>((entt::entity)entity, sprite);
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
