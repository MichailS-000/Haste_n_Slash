#pragma once
#include <SDL_mixer.h>
#include "../resources/resource_container.hpp"

class AudioManager
{
private:
	ResourceContainer* container;
	Mix_Music* currentPlayngMusic;
};