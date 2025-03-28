#pragma once

class Time
{
public:
	static void Update();
	static float GetDeltaTime();
	static float GetTime();
private:
	static float lastTime;
	static float deltaTime;
	static float time;
	Time();
};