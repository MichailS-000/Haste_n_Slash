#include "logger.hpp"
#include <ctime>
#include <chrono>

std::ofstream Logger::stream = std::ofstream(".\\log.txt", std::ios::trunc);

void Logger::Log(std::string&& string)
{
	std::string msg = std::format("{} Log: {}", GetCurrentTime(), string);

	stream << msg << std::endl;
	std::cout << msg << std::endl;
}

void Logger::LogWarning(int code, std::string&& string)
{
	std::string msg = std::format("{} Warning WR{}: {}", GetCurrentTime(), code, string);

	stream << msg << std::endl;
	std::cout << msg << std::endl;
}

void Logger::LogError(int code, std::string&& string)
{
	std::string msg = std::format("{} ERROR ERR{}: {}", GetCurrentTime(), code, string);

	stream << msg << std::endl;
	std::cout << msg << std::endl;
}

std::string Logger::GetCurrentTime()
{
	std::chrono::time_point now = std::chrono::system_clock::now();
	std::time_t time = std::chrono::system_clock::to_time_t(now);
	std::tm* currentTime = std::localtime(&time);

	return std::format("{}:{}:{}", currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec);
}

Logger::~Logger()
{
	stream.close();
}
