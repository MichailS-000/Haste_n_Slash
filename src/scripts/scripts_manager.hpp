#pragma once
#include "../application/scripts_execution_enviroment.hpp"
#include "compiled_script.hpp"

class ScriptsManager
{
private:
	ScriptsExecutionEnviroment* scriptsEnv;
	SCRIPT_BINARY_PERMISSIONS_TYPE ParsePermissions(std::vector<std::string>& permissions);
	void LinkScriptsDependencies(lua_State* state, SCRIPT_BINARY_PERMISSIONS_TYPE permissions);
public:
	ScriptsManager(ScriptsExecutionEnviroment* env);
	~ScriptsManager();
	void UpdateScripts();
	bool CallFunction(const char* functionName, lua_State* state);
	CompiledScript* CompileScript(const std::string& source, std::vector<std::string>& permissions);
};