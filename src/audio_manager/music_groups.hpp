#pragma once

#include <string>
#include <unordered_map>
#include <vector>

class MusicGroups
{
private:
	unsigned int lastTrack = 0;
	std::unordered_map<std::string, std::vector<std::string>> musicGroups;
public:
	void AddMusicToGroup(std::string group, std::string trackName);
	const std::string& GetRandomFromGroup(std::string group);
};