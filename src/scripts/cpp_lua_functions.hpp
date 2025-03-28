#pragma once

#include "../application/ScriptsExecutionEnviroment.hpp"
#include <lua.hpp>

extern void LinkGenericLib(lua_State* state, ScriptsExecutionEnviroment* env);

extern void LinkEntityLib(lua_State* state, ScriptsExecutionEnviroment* env);
extern void LinkInputLib(lua_State* state, ScriptsExecutionEnviroment* env);
extern void LinkGraphicsLib(lua_State* state, ScriptsExecutionEnviroment* env);