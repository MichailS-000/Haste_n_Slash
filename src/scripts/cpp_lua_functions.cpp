#include "cpp_lua_functions.hpp"

#include "../components/graphic.hpp"

#include <LuaBridge/LuaBridge.h>
#include <lua.hpp>

void LinkEntityLib(lua_State* state, ExecutionEnviroment* env)
{
	luabridge::getGlobalNamespace(state)
		.beginNamespace("entity")
		.addFunction("addEntity", [env = env]() 
			{
				return (int)env->applicationRegistry->create();
			})
		.addFunction("addComponent", [env = env](std::string componentName, int entity)
			{
			})
		.addFunction("setImage", [env = env](std::string imageName, int entity) 
			{
				auto img = env->resourcesContainer->GetImageInst(imageName);
				env->applicationRegistry->emplace<components::Image>(static_cast<entt::entity>(entity), img);
			})
		.endNamespace();
}

void LinkInputLib(lua_State* state, ExecutionEnviroment* env)
{
}
