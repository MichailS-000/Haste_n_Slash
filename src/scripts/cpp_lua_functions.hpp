#pragma once

#include "../application/scripts_execution_enviroment.hpp"
#include <lua.hpp>

extern void LinkGenericLib(lua_State* state, ScriptsExecutionEnviroment* env);

extern void LinkEntityLib(lua_State* state, ScriptsExecutionEnviroment* env);
extern void LinkInputLib(lua_State* state, ScriptsExecutionEnviroment* env);
extern void LinkGraphicsLib(lua_State* state, ScriptsExecutionEnviroment* env);
extern void LinkAudioLib(lua_State* state, ScriptsExecutionEnviroment* env);