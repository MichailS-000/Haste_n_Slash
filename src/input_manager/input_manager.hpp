#pragma once
#include <map>
#include <SDL3/SDL.h>

class InputManager
{
private:
	enum KeyPos { No, Down, Press, Up };
	std::map <std::string, SDL_Scancode> registredKeys;
	std::map <SDL_Scancode, KeyPos> keysState;
public:
	bool GetKeyUp(std::string&& key);
	bool GetKeyUp(SDL_Scancode key);
	bool GetKey(std::string&& key);
	bool GetKey(SDL_Scancode key);
	bool GetKeyDown(std::string&& key);
	bool GetKeyDown(SDL_Scancode key);
	InputManager();
	void Update();
	~InputManager();
};