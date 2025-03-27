#include "input_manager.hpp"

#define REGISTER_KEY(x) std::pair<std::string, SDL_Scancode>{#x, SDL_SCANCODE_##x}
#define REGISTER_SPECIAL_KEY(x, y) std::pair<std::string, SDL_Scancode>{#x, SDL_SCANCODE_##y}

bool InputManager::GetKeyUp(std::string&& key)
{
	return keysState[registredKeys[key]] == KeyPos::Up;
}

bool InputManager::GetKeyUp(SDL_Scancode key)
{
	return keysState[key] == KeyPos::Up;
}

bool InputManager::GetKey(std::string&& key)
{
	return keysState[registredKeys[key]] == KeyPos::Press;;
}

bool InputManager::GetKey(SDL_Scancode key)
{
	return keysState[key] == KeyPos::Press;
}

bool InputManager::GetKeyDown(std::string&& key)
{
	return keysState[registredKeys[key]] == KeyPos::Down;
}

bool InputManager::GetKeyDown(SDL_Scancode key)
{
	return keysState[key] == KeyPos::Down;
}

InputManager::InputManager()
{
	registredKeys = 
	{
		REGISTER_KEY(W),
		REGISTER_KEY(A),
		REGISTER_KEY(S),
		REGISTER_KEY(D),
		REGISTER_KEY(ESCAPE),
		REGISTER_KEY(SPACE),
		REGISTER_KEY(LEFT),
		REGISTER_KEY(UP),
		REGISTER_KEY(RIGHT),
		REGISTER_KEY(DOWN),
		REGISTER_KEY(Q),
		REGISTER_KEY(E),
		REGISTER_KEY(F),
		REGISTER_KEY(R),
		REGISTER_KEY(X),
		REGISTER_KEY(Z),
		REGISTER_KEY(C),
		REGISTER_KEY(V),
		REGISTER_KEY(1),
		REGISTER_KEY(2),
		REGISTER_KEY(3),
		REGISTER_KEY(4)
	};

	for (const auto& [keyStr, keyCode] : registredKeys)
	{
		keysState.emplace(std::pair<SDL_Scancode, KeyPos>{keyCode, KeyPos::No});
	}
}

void InputManager::Update()
{
	const bool* currentKeyboardState = SDL_GetKeyboardState(NULL);

	for (const auto& [keyCode, keyState] : keysState)
	{
		switch (keyState)
		{
		case KeyPos::No:
			if (currentKeyboardState[keyCode])
			{
				keysState[keyCode] = KeyPos::Down;
			}
			break;
		case KeyPos::Down:
			if (currentKeyboardState[keyCode])
			{
				keysState[keyCode] = KeyPos::Press;
			}
			else
			{
				keysState[keyCode] = KeyPos::Up;
			}
			break;
		case KeyPos::Press:
			if (!currentKeyboardState[keyCode])
			{
				keysState[keyCode] = KeyPos::Up;
			}
			break;
		case KeyPos::Up:
			if (currentKeyboardState[keyCode])
			{
				keysState[keyCode] = KeyPos::Down;
			}
			else
			{
				keysState[keyCode] = KeyPos::No;
			}
			break;
		default:
			break;
		}
	}
}

InputManager::~InputManager()
{
}
