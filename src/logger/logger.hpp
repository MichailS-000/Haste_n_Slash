#pragma once
#include <iostream>
#include <fstream>
#include <string>

class Logger
{
public:
	static void Log(std::string&& string);
	static void LogWarning(int code, std::string&& string);
	static void LogError(int code, std::string&& string);
private:
	static std::string GetCurrentTime();
	static std::ofstream stream;
	Logger();
	~Logger();
};
