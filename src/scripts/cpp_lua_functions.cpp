#include "cpp_lua_functions.hpp"

#include "../components/components.hpp"

#include <LuaBridge/LuaBridge.h>
#include <lua.hpp>

void LinkEntityLib(lua_State* state, ScriptsExecutionEnviroment* env)
{
	luabridge::getGlobalNamespace(state)
		.beginNamespace("entity")
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
