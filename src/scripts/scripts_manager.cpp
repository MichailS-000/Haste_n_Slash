#include "scripts_manager.hpp"

#include "../components/script.hpp"
#include "../logger/logger.hpp"
#include "cpp_lua_functions.hpp"

#include <LuaBridge/LuaBridge.h>
#include <unordered_map>
#include <format>

SCRIPT_BINARY_PERMISSIONS_TYPE ScriptsManager::ParsePermissions(std::vector<std::string>& permissions)
{
    SCRIPT_BINARY_PERMISSIONS_TYPE permissionsBinary = 0;

    static const std::unordered_map<std::string, SCRIPT_BINARY_PERMISSIONS_TYPE> permissionMap = {
        {"start", ScriptPermissions::StartFunction},
        {"update", ScriptPermissions::UpdateFunction},
        {"entity", ScriptPermissions::Entity},
        {"input", ScriptPermissions::Input},
		{"graphics", ScriptPermissions::Graphics}
    };

    for (const auto& permission : permissions)
    {
        auto it = permissionMap.find(permission);
        if (it != permissionMap.end())
        {
            permissionsBinary |= it->second;
        }
		else
		{
			Logger::LogWarning(1, std::format("Permission \"{}\" dont registred", permission));
		}
    }

    return permissionsBinary;
}

void ScriptsManager::LinkScriptsDependencies(lua_State* state, SCRIPT_BINARY_PERMISSIONS_TYPE permisssions)
{
	luaL_openlibs(state);

	if (permisssions | ScriptPermissions::Entity)
	{
		LinkEntityLib(state, scriptsEnv);
	}
	if (permisssions | ScriptPermissions::Graphics)
	{
		LinkGraphicsLib(state, scriptsEnv);
	}
	if (permisssions | ScriptPermissions::Input)
	{
		LinkInputLib(state, scriptsEnv);
	}
}

ScriptsManager::ScriptsManager(ScriptsExecutionEnviroment* env) : scriptsEnv(env)
{
}

ScriptsManager::~ScriptsManager()
{
	for (auto& [_, script] : scripts)
	{
		lua_close(script.state);
	}
}

void ScriptsManager::UpdateScripts()
{
	auto view = scriptsEnv->applicationRegistry->view<components::Script>();

	for (auto entity : view)
	{
		scriptsEnv->currentUpdatingEntity = entity;

		components::Script& scriptRef = view.get<components::Script>(entity);
		CompiledScript& script = scripts[scriptRef.name];
		
		if ((script.permissions & ScriptPermissions::StartFunction) && scriptRef.initState == 0)
		{
			CallFunction("Start", script.state);
			scriptRef.initState = 1;
		}
		
		if (script.permissions & ScriptPermissions::UpdateFunction)
		{
			CallFunction("Update", script.state);
		}
	}
}

void ScriptsManager::CallFunction(const char* functionName, lua_State* state)
{
	try
	{
		auto func = luabridge::getGlobal(state, functionName);

		auto res = luabridge::call(func);

		if (res.hasFailed())
		{
			throw std::runtime_error(std::format("Lua error {}\n{}", res.errorCode().value(), res.errorMessage()));
		}
	}
	catch (luabridge::LuaException& e)
	{
		Logger::LogError(3, e.what());
	}
	catch (std::runtime_error& e)
	{
		Logger::LogError(3, e.what());
	}
}

void ScriptsManager::CompileScripts()
{
	UncompiledScript* uncompiledScript;

	while ((uncompiledScript = scriptsEnv->resourcesContainer->GetNextUncompiledScript()) != nullptr)
	{
		CompiledScript script;
		script.name = uncompiledScript->name;
		script.permissions = ParsePermissions(uncompiledScript->permissions);
		script.state = luaL_newstate();
		LinkScriptsDependencies(script.state, script.permissions);

		luaL_dofile(script.state, uncompiledScript->sourcePath.c_str());
		scripts.emplace(script.name, script);

		delete uncompiledScript;
	}
}
