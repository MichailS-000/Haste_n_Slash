#include "scripts_manager.hpp"

#include "../components/script.hpp"
#include "../logger/logger.hpp"
#include "cpp_lua_functions.hpp"
#include "../resources/resource_accessor.hpp"

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
		{"graphics", ScriptPermissions::Graphics},
		{"audio", ScriptPermissions::Audio},
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

void ScriptsManager::LinkScriptsDependencies(lua_State* state, SCRIPT_BINARY_PERMISSIONS_TYPE permissions)
{
	luaL_openlibs(state);
	LinkGenericLib(state, scriptsEnv);

	if (permissions & ScriptPermissions::Entity)
	{
		LinkEntityLib(state, scriptsEnv);
	}
	if (permissions & ScriptPermissions::Graphics)
	{
		LinkGraphicsLib(state, scriptsEnv);
	}
	if (permissions & ScriptPermissions::Input)
	{
		LinkInputLib(state, scriptsEnv);
	}
	if (permissions & ScriptPermissions::Audio)
	{
		LinkAudioLib(state, scriptsEnv);
	}
}

ScriptsManager::ScriptsManager(ScriptsExecutionEnviroment* env) : scriptsEnv(env)
{
}

ScriptsManager::~ScriptsManager()
{
}

void ScriptsManager::UpdateScripts()
{
	auto view = scriptsEnv->applicationRegistry->view<components::Script>();

	for (auto entity : view)
	{
		scriptsEnv->currentUpdatingEntity = entity;

		components::Script& scriptRef = view.get<components::Script>(entity);
		CompiledScript* script = scriptsEnv->resources->Get<CompiledScript>(scriptRef.name);

		if (script == nullptr)
		{
			Logger::LogError(3, std::format("Script \"{}\" does not exist or not presented in resources.json", scriptRef.name));
			scriptsEnv->applicationRegistry->erase<components::Script>(entity);
			continue;
		}
		
		if ((script->permissions & ScriptPermissions::StartFunction) && scriptRef.initState == 0)
		{
			if (CallFunction("Start", script->state))
			{
				scriptRef.initState = 1;
			}
			else
			{
				Logger::Log(std::format("Error trace when Start function called from \"{}\" entityID: {}", script->name, (int)entity));
				scriptsEnv->applicationRegistry->erase<components::Script>(entity);
			}
		}
		
		if (script->permissions & ScriptPermissions::UpdateFunction)
		{
			if (!CallFunction("Update", script->state))
			{
				Logger::Log(std::format("Error trace when Update function called from \"{}\" entityID: {}", script->name, (int)entity));
				scriptsEnv->applicationRegistry->erase<components::Script>(entity);
			}
		}
	}
}

bool ScriptsManager::CallFunction(const char* functionName, lua_State* state)
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
		return false;
	}
	catch (std::runtime_error& e)
	{
		Logger::LogError(3, e.what());
		return false;
	}

	return true;
}

CompiledScript* ScriptsManager::CompileScript(const std::string& source, std::vector<std::string>& permissions)
{
	CompiledScript* script = new CompiledScript();
	script->permissions = ParsePermissions(permissions);
	script->state = luaL_newstate();
	LinkScriptsDependencies(script->state, script->permissions);

	if (luaL_dofile(script->state, source.c_str()) != 0)
	{
		if (lua_isstring(script->state, lua_gettop(script->state)))
		{
			std::string error = lua_tostring(script->state, lua_gettop(script->state));
			lua_pop(script->state, 1);
			Logger::LogError(3, std::format("Error while lua dofile: {}", error));
		}
		else
		{
			Logger::LogError(3, std::format("Error while lua dofile: unknown error"));
		}
		return nullptr;
	}

	return script;
}
