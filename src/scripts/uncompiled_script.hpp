#pragma once
#include <vector>
#include <string>

struct UncompiledScript
{
	std::string name = "null";
	std::vector<std::string> permissions;
	std::string sourcePath;
};