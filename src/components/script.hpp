#pragma once
#include <string>
#include <lua.hpp>
#include <string>

#define SCRIPT_BINARY_PERMISSIONS_TYPE Uint16

namespace components
{
	enum ScriptPermissions
	{
		No = 0b00000000,
		StartFunction = 0b1,
		UpdateFunction = 0b10,
		Input = 0b100,
		Entity = 0b1000,
	};

	struct Script
	{
		char initState = 0;
		std::string name = "null";
		SCRIPT_BINARY_PERMISSIONS_TYPE permissions;
		lua_State* state = nullptr;
	};
}
