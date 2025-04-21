#include "cpp_lua_functions.hpp"

#include "../components/components.hpp"
#include "../application/program_time.hpp"
#include "../input_manager/input_manager.hpp"
#include "../audio_manager/audio_manager.hpp"
#include "../logger/logger.hpp"

#include <LuaBridge/LuaBridge.h>
#include <lua.hpp>

#include <unordered_map>
#include <format>

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
	auto variableTable = luabridge::newTable(state);

	luabridge::getGlobalNamespace(state)
		.addFunction("setVariable", [env = env, variables = variableTable](std::string varName, luabridge::LuaRef value)
			{
				auto& script = env->applicationRegistry->get<components::Script>(env->currentUpdatingEntity);

				variables[std::format("{}{}", varName, (int)env->currentUpdatingEntity)] = value;
			})
		.addFunction("getVariable", [env = env, variables = variableTable](std::string varName)
			{
				auto& script = env->applicationRegistry->get<components::Script>(env->currentUpdatingEntity);

				return variables[std::format("{}{}", varName, (int)env->currentUpdatingEntity)];
			})
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
		.beginClass<components::Transform>("Transform")
			.addProperty("positionX", &components::Transform::positionX, &components::Transform::positionX)
			.addProperty("positionY", &components::Transform::positionY, &components::Transform::positionY)
			.addProperty("scaleX", &components::Transform::scaleX, &components::Transform::scaleX)
			.addProperty("scaleY", &components::Transform::scaleY, &components::Transform::scaleY)
		.endClass()
		.addFunction("getComponent", [env = env, state = state](int entity, std::string componentName) -> luabridge::LuaRef
			{
				if (!ValidateEntity(*env->applicationRegistry, (entt::entity)entity))
				{
					throw std::runtime_error("Trying to handle invalid entity");
				}

				if (componentName == "Transform")
				{
					if (!HasComponent<components::Transform>(*env->applicationRegistry, (entt::entity)entity))
					{
						throw std::runtime_error(std::format("Entity id:{} does not contains Transform", entity));
					}

					auto& ptr = env->applicationRegistry->get<components::Transform>((entt::entity)entity);

					return luabridge::LuaRef(state, &ptr);
				}

				// TODO: Make getters for all components

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
