#include <iostream>
#include <SDL3/SDL.h>

int main()
{
	SDL_Window* window = SDL_CreateWindow("Haste & Slash", 800, 600, SDL_WINDOW_OPENGL);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, "renderer");

	bool shouldClose = false;

	while (!shouldClose)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_EVENT_QUIT:
					shouldClose = true;
			}

			SDL_RenderClear(renderer);

			SDL_RenderPresent(renderer);
		}
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	return 0;
}