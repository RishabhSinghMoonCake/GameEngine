#include <SDL3/SDl.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

using namespace std;



struct SDLState
{
	SDL_Window* window;
	SDL_Renderer* renderer;
};
void cleanUp(SDLState &state);

int main(int argc, char *argv[])
{
	SDLState state;

	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Failed to initialize SDL", nullptr);
		return 1;
	}

	//create the window

	int width = 800;
	int height = 800;

	state.window = SDL_CreateWindow("Game Engine",width, height,0);
	if (!state.window)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Failed to create window", nullptr);
		cleanUp(state);
		return 1;
	}

	//create the renderer
	state.renderer = SDL_CreateRenderer(state.window, nullptr);
	if (!state.renderer)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Failed to create renderer", nullptr);
		cleanUp(state);
		return 1;
	}

	//create the game main loop
	bool running = true;
	while (running)
	{
		SDL_Event event{ 0 };
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_EVENT_QUIT:
				running = false;
				break;
			default:
				break;
			}
		}
		
		//perform drawing commands
		SDL_SetRenderDrawColor(state.renderer, 255, 255, 255, 255);
		SDL_RenderClear(state.renderer);



		//swap buffers and present
		SDL_RenderPresent(state.renderer);
	}

	cleanUp(state);
	return 0;
}


void cleanUp(SDLState& state)
{
	SDL_DestroyRenderer(state.renderer);
	SDL_DestroyWindow(state.window);
	SDL_Quit();
}