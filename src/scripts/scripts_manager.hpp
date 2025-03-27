#pragma once
#include "../haste_n_slash.hpp"
#include "../components/script.hpp"
#include <lua.hpp>

class ScriptsManager
{
private:
	std::map<std::string, components::Script> scripts;
	ExecutationEnviroment* scriptsEnv;
	SCRIPT_BINARY_PERMISSIONS_TYPE ParsePermissions(std::vector<std::string>& permissions);
	void LinkScriptsDependencies(lua_State* state, SCRIPT_BINARY_PERMISSIONS_TYPE permisssions);
public:
	ScriptsManager(ExecutationEnviroment* env);
	~ScriptsManager();
	void UpdateScripts();
	components::Script GetScriptInst(std::string&& scriptName);
	void CallFunction(const char* functionName, lua_State* state);
	void CompileScripts();
};