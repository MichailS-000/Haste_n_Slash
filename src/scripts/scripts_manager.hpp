#pragma once
#include "../application/ScriptsExecutionEnviroment.hpp"
#include "compiled_script.hpp"

class ScriptsManager
{
private:
	std::map<std::string, CompiledScript> scripts;
	ScriptsExecutionEnviroment* scriptsEnv;
	SCRIPT_BINARY_PERMISSIONS_TYPE ParsePermissions(std::vector<std::string>& permissions);
	void LinkScriptsDependencies(lua_State* state, SCRIPT_BINARY_PERMISSIONS_TYPE permisssions);
public:
	ScriptsManager(ScriptsExecutionEnviroment* env);
	~ScriptsManager();
	void UpdateScripts();
	void CallFunction(const char* functionName, lua_State* state);
	void CompileScripts();
};