#include "cpp_lua_functions.hpp"

#include "../components/graphic.hpp"

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
		.endNamespace();
}

void LinkInputLib(lua_State* state, ScriptsExecutionEnviroment* env)
{
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
