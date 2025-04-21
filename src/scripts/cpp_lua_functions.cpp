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

class EntityHelper
{
private:
	static inline bool registerFlag = false;

	static inline std::unordered_map<std::string, std::function<void(EntityHelper*)>> removeMap;
	static inline std::unordered_map<std::string, std::function<luabridge::LuaRef(EntityHelper*)>> addMap;
	static inline std::unordered_map<std::string, std::function<luabridge::LuaRef(EntityHelper*)>> getMap;
	static inline std::unordered_map<std::string, std::function<EntityHelper(entt::registry*, lua_State*)>> getEntityMap;

	entt::entity entity;
	entt::registry* registry;
	lua_State* state;

	template<typename T>
	static void RegisterComponent()
	{
		std::string name = ComponentTraits<T>::name;

		removeMap[name] = [](EntityHelper* self) { self->EraseComponent<T>(); };
		addMap[name] = [](EntityHelper* self) { return self->ApplyComponent<T>(); };
		getMap[name] = [](EntityHelper* self) { return self->ReturnComponent<T>(); };
		getEntityMap[name] = [](entt::registry* reg, lua_State* state) { return GetFirstEntityWithComponent<T>(reg, state); };

		Logger::Log(std::format("Component \"{}\" registered", name));
	}

	template<typename T>
	static EntityHelper GetFirstEntityWithComponent(entt::registry* reg, lua_State* state)
	{
		auto view = reg->view<T>();

		if (view.size() > 0)
		{
			return EntityHelper(view.front(), reg, state);
		}
		else
		{
			throw std::runtime_error(std::format("Registry does not contains entities with component \"{}\"", ComponentTraits<T>::name));
		}
	}

public:

	static EntityHelper GetEntityWithComponent(entt::registry* reg, lua_State* state, std::string& componentName)
	{
		if (auto it = getEntityMap.find(componentName); it != getEntityMap.end())
		{
			return it->second(reg, state);
		}
		else
		{
			Logger::LogWarning(4, std::format("Component \"{}\" does not exist", componentName));
			return luabridge::LuaRef(state);
		}
	}

	template<typename... Components>
	static void RegisterAllComponents()
	{
		if (registerFlag)
			return;

		(RegisterComponent<Components>(), ...);
		registerFlag = true;
	}	

	EntityHelper(entt::entity entity, entt::registry* registry, lua_State* state) : entity(entity), registry(registry), state(state)
	{

	}
	
	int GetId() const
	{
		return (int)entity;
	}

	void Destroy()
	{
		registry->destroy(entity);
	}

	luabridge::LuaRef GetTransform()
	{
		return ReturnComponent<components::Transform>();
	}

	template<typename T>
	inline luabridge::LuaRef ReturnComponent()
	{
		if (registry->all_of<T>(entity))
		{
			auto& component = registry->get<T>(entity);

			return luabridge::LuaRef(state, &component);
		}
		else
		{
			Logger::LogWarning(4, std::format("Entity id: {} does not contains {}", (int)entity, typeid(T).name()));
			return luabridge::LuaRef(state);
		}
	}

	template<typename T>
	inline luabridge::LuaRef ApplyComponent()
	{
		if (!registry->all_of<T>(entity))
		{
			auto& component = registry->emplace<T>(entity);

			return luabridge::LuaRef(state, &component);
		}
		else
		{
			Logger::LogWarning(4, std::format("Entity id: {} already contains {}", (int)entity, typeid(T).name()));
			return luabridge::LuaRef(state);
		}
	}

	template<typename T>
	inline void EraseComponent()
	{
		if (registry->all_of<T>(entity))
		{
			registry->erase<T>(entity);
		}
		else
		{
			Logger::LogWarning(4, std::format("Entity id: {} does not contains {}", (int)entity, ComponentTraits<T>::name));
		}
	}

	void RemoveComponent(std::string componentName)
	{
		if (auto it = removeMap.find(componentName); it != removeMap.end())
		{
			it->second(this);
		}
		else
		{
			Logger::LogWarning(4, std::format("Component \"{}\" does not exist", componentName));
		}
	}

	luabridge::LuaRef AddComponent(const std::string& componentName)
	{
		if (auto it = addMap.find(componentName); it != addMap.end())
		{
			return it->second(this);
		}
		else
		{
			Logger::LogWarning(4, std::format("Component \"{}\" does not exist", componentName));
			return luabridge::LuaRef(state);
		}
	}

	luabridge::LuaRef GetComponent(const std::string& componentName)
	{
		if (auto it = getMap.find(componentName); it != getMap.end())
		{
			return it->second(this);
		}
		else
		{
			Logger::LogWarning(4, std::format("Component \"{}\" does not exist", componentName));
			return luabridge::LuaRef(state);
		}
	}
};

static ScriptsExecutionEnviroment* g_env = nullptr;

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
	EntityHelper::RegisterAllComponents<
		components::AnimatedSprite,
		components::Background,
		components::Camera,
		components::RectTransform,
		components::Script,
		components::Sprite,
		components::Transform>();

	g_env = env;

	auto variableTable = luabridge::newTable(state);

	try
	{
		luabridge::getGlobalNamespace(state)
			.beginClass<EntityHelper>("Entity")
				.addFunction("getID", &EntityHelper::GetId)
				.addFunction("getComponent", &EntityHelper::GetComponent)
				.addFunction("addComponent", &EntityHelper::AddComponent)
				.addFunction("removeComponent", &EntityHelper::RemoveComponent)
				.addFunction("destroy", &EntityHelper::Destroy)
				.addProperty("transform", [](EntityHelper* entity) { return entity->GetTransform(); })
			.endClass()
			.addFunction("addEntity", [env = env, state = state]() 
				{
					auto entity = env->applicationRegistry->create();
					return EntityHelper(entity, env->applicationRegistry, state);
				})
			.addFunction("getCurrentEntity", [env = env, state = state]
				{
					return EntityHelper(env->currentUpdatingEntity, env->applicationRegistry, state);
				})
			.addFunction("getFirstEntityWithComponent", [env = env, state = state](std::string componentName) 
				{
					return EntityHelper::GetEntityWithComponent(env->applicationRegistry, state, componentName);
				})
			.addFunction("setVariable", [env = env, variables = variableTable](std::string varName, luabridge::LuaRef value)
				{
					variables[std::format("{}{}", varName, (int)env->currentUpdatingEntity)] = value;
				})
			.addFunction("getVariable", [env = env, variables = variableTable](std::string varName)
				{
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
	catch (std::logic_error& e)
	{
		Logger::LogError(3, std::format("Lua linking error with: {}", e.what()));
	}
	catch (luabridge::LuaException& e)
	{
		Logger::LogError(3, std::format("Lua linking error with: {}", e.what()));
	}
}

void LinkEntityLib(lua_State* state, ScriptsExecutionEnviroment* env)
{
	g_env = env;

	try
	{
		luabridge::getGlobalNamespace(state)
		.beginNamespace("entity")
			.beginClass<components::ComponentBase>("ComponentBase")
				.addProperty("enabled", &components::ComponentBase::enabled, &components::ComponentBase::enabled)
			.endClass()
			.deriveClass<components::Transform, components::ComponentBase>("Transform")
				.addProperty("positionX", &components::Transform::positionX, &components::Transform::positionX)
				.addProperty("positionY", &components::Transform::positionY, &components::Transform::positionY)
				.addProperty("scaleX", &components::Transform::scaleX, &components::Transform::scaleX)
				.addProperty("scaleY", &components::Transform::scaleY, &components::Transform::scaleY)
				.addFunction("movePosition", [](components::Transform& transform, float x, float y)
					{
						transform.positionX += x;
						transform.positionY += y;
					})
			.endClass()
			.deriveClass<components::Camera, components::ComponentBase>("Camera")
				.addProperty("scale", &components::Camera::scale, &components::Camera::scale)
			.endClass()
			.deriveClass<components::Sprite, components::ComponentBase>("Sprite")
				.addProperty("textureName", &components::Sprite::textureName, &components::Sprite::textureName)
			.endClass()
				.deriveClass<components::Background, components::ComponentBase>("Background")
			.addProperty("textureName", &components::Background::textureName, &components::Background::textureName)
			.endClass()
		.endNamespace();
	}
	catch (std::logic_error& e)
	{
		Logger::LogError(3, std::format("Lua linking error with: {}", e.what()));
	}
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
