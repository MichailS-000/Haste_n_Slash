#include "program_time.hpp"
#include <SDL3/SDL.h>

float Time::deltaTime = 0;
float Time::lastTime = 0;
float Time::time = 0;

void Time::Update()
{
	time = (SDL_GetTicks() / 1000.f);
	deltaTime = time - lastTime;
	lastTime = time;
}

float Time::GetDeltaTime()
{
	return deltaTime;
}

float Time::GetTime()
{
	return time;
}
