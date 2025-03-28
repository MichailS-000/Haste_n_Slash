#pragma once
#include <lua.hpp>

#define SCRIPT_BINARY_PERMISSIONS_TYPE unsigned short

enum ScriptPermissions
{
	No = 0b00000000,
	StartFunction = 0b1,
	UpdateFunction = 0b10,
	Input = 0b100,
	Entity = 0b1000,
	Graphics = 0b10000
};

struct CompiledScript
{
	std::string name = "null";
	SCRIPT_BINARY_PERMISSIONS_TYPE permissions = ScriptPermissions::No;
	lua_State* state = nullptr;
};