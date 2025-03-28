#pragma once
#include "../application/ScriptsExecutionEnviroment.hpp"
#include "../components/script.hpp"

class ScriptsManager
{
private:
	std::map<std::string, components::Script> scripts;
	ScriptsExecutionEnviroment* scriptsEnv;
	SCRIPT_BINARY_PERMISSIONS_TYPE ParsePermissions(std::vector<std::string>& permissions);
	void LinkScriptsDependencies(lua_State* state, SCRIPT_BINARY_PERMISSIONS_TYPE permisssions);
public:
	ScriptsManager(ScriptsExecutionEnviroment* env);
	~ScriptsManager();
	void UpdateScripts();
	components::Script GetScriptInst(std::string&& scriptName);
	void CallFunction(const char* functionName, lua_State* state);
	void CompileScripts();
};