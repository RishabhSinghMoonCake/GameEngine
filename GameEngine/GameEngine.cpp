#include <SDL3/SDl.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

using namespace std;



struct SDLState
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	int width, height, logW, logH;
};
void cleanUp(SDLState &state);

bool initialize(SDLState& state);

int main(int argc, char *argv[])
{
	SDLState state;

	if (initialize(state)) //initialize SDL and create the window and renderer
	{
		//true for failure
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Failed to initialize SDL", nullptr);
		return 1;
	}
	//load game assets
	SDL_Texture* idleTex = IMG_LoadTexture(state.renderer, "assets/idle.png"); //preload textures into gpu, [you can also use sdl_surface instead of texture to load into memory]
	if (!idleTex)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Failed to load idle texture", nullptr);
		cleanUp(state);
		return 1;
	}
	SDL_SetTextureScaleMode(idleTex, SDL_SCALEMODE_NEAREST); //set the scale mode for the texture

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
			case SDL_EVENT_WINDOW_RESIZED:
			{
				state.width = event.window.data1;
				state.height = event.window.data2;
				break;
			}
			default:
				break;
			}
		}
		
		//perform drawing commands
		SDL_SetRenderDrawColor(state.renderer, 20, 20, 20, 255);
		SDL_RenderClear(state.renderer);

		SDL_FRect src{
			.x = 0.0f,
			.y = 0.0f,
			.w = 32.0f,
			.h = 32.0f
		};

		SDL_FRect dst{
			.x = 0.0f,
			.y = 0.0f,
			.w = 32.0f,
			.h = 32.0f
		};

		SDL_RenderTexture(state.renderer, idleTex, &src, &dst); //draw the idle texture to the renderer

		//swap buffers and present
		SDL_RenderPresent(state.renderer);
	}
	SDL_DestroyTexture(idleTex); //clean up the texture 
	cleanUp(state);
	return 0;
}

bool initialize(SDLState& state)
{
	state.width = 1600;
	state.height = 800;
	state.logH = 320;
	state.logW = 640;


	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Failed to initialize SDL", nullptr);
		return 1;
	}

	//create the window

	int width = 800;
	int height = 800;

	state.window = SDL_CreateWindow("Game Engine", width, height, SDL_WINDOW_RESIZABLE);
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

	//configure presentation

	int logW = 640;
	int logH = 320;
	SDL_SetRenderLogicalPresentation(state.renderer, logW, logH, SDL_LOGICAL_PRESENTATION_LETTERBOX); //get the logical presentation size
	return 0; //0 for success and 1 for failure
}


void cleanUp(SDLState& state)
{
	SDL_DestroyRenderer(state.renderer);
	SDL_DestroyWindow(state.window);
	SDL_Quit();
}