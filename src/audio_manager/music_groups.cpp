#include "music_groups.hpp"

void MusicGroups::AddMusicToGroup(std::string group, std::string trackName)
{
	musicGroups[group].push_back(trackName);
}

const std::string& MusicGroups::GetRandomFromGroup(std::string group)
{
	unsigned int randomTrack = rand() % musicGroups[group].size();
	if (randomTrack == lastTrack)
	{
		randomTrack = (randomTrack + 1) % musicGroups[group].size();
	}

	lastTrack = randomTrack;
	return musicGroups[group][randomTrack];
}
