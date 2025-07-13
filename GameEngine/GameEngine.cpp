#include <SDL3/SDl.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <string>
#include "gameObject.h"
#include <array>

#include <glm/glm.hpp>

using namespace std;




struct SDLState
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	int width, height, logW, logH;
};
void cleanUp(SDLState &state);

bool initialize(SDLState& state);

const size_t LAYER_IDX_LEVEL = 0; //index of the level layer
const size_t LAYER_IDX_CHARACTERS = 1; //index of the characters layer

struct GameState
{
	std::array<std::vector<GameObject>, 2> layers; //array of layers, each layer is a vector of game objects
};

struct Resources
{
	const int ANIM_PLAYER_IDLE = 0; //index of idle animation
	std::vector<Animation> playerAnims; //vector of player animations

	std::vector<SDL_Texture*> textures; //vector of textures
	SDL_Texture* idleTex; //texture for idle animation

	SDL_Texture* loadTexture(SDL_Renderer *renderer, const std::string& path)
	{
		//load game assets
		SDL_Texture* tex = IMG_LoadTexture(renderer, path.c_str()); //preload textures into gpu, [you can also use sdl_surface instead of texture to load into memory]

		SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST); //set the scale mode for the texture
		textures.push_back(tex); //add the texture to the vector of textures
		return tex; //return the pointer to the texture
	}

	void load(SDLState& state)
	{
		playerAnims.resize(5);
		playerAnims[ANIM_PLAYER_IDLE] = Animation(8, 1.6f); //idle animation with 1 frame and 0.5 seconds length

		idleTex = loadTexture(state.renderer, "assets/idle.png"); //load the idle texture
	}

	void unload()
	{
		for (auto& tex : textures) //iterate through all the textures
		{
			SDL_DestroyTexture(tex); //destroy the texture
		}
		textures.clear(); //clear the vector of textures
	}
};

int main(int argc, char *argv[])
{
	SDLState state;

	if (initialize(state)) //initialize SDL and create the window and renderer
	{
		//true for failure
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Failed to initialize SDL", nullptr);
		return 1;
	}
	
	//load the game resources
	Resources res;
	res.load(state); //load the resources



	//setup game data
	const bool* keys = SDL_GetKeyboardState(nullptr); //get the current keyboard state
	float playerX = 0.0f; //player position on x-axis
	float playerY = 0.0f; //player position on y-axis

	const float floor = state.logH;
	uint64_t prevTime = SDL_GetTicks(); //get the previous time in milliseconds

	//variables
	bool flipHorizontal = false;

	//create the game main loop
	bool running = true;
	while (running)
	{
		uint64_t nowTime = SDL_GetTicks(); //get the current time in milliseconds
		float deltaTime = (nowTime - prevTime) / 1000.0f; //calculate the time elapsed since the last frame in seconds
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
		

		//handle movement
		float moveAmount = 0;
		float playerXSpeed = 175.0f; //speed of the player on x-axis

		if (keys[SDL_SCANCODE_A])
		{
			moveAmount += -playerXSpeed;
			flipHorizontal = true; //flip the sprite horizontally when moving left
		}
		else if (keys[SDL_SCANCODE_D])
		{
			moveAmount += playerXSpeed;
			flipHorizontal = false; //do not flip the sprite when moving right
		}

		playerX += moveAmount * deltaTime; //update the player position based on the time elapsed since the last frame
		
		//perform drawing commands
		SDL_SetRenderDrawColor(state.renderer, 20, 20, 20, 255);
		SDL_RenderClear(state.renderer);

		const float spriteSize = 32.0f; //size of the sprite

		SDL_FRect src{
			.x = 0.0f,
			.y = 0.0f,
			.w = spriteSize,
			.h = spriteSize
		};

		SDL_FRect dst{
			.x = playerX,
			.y = floor-spriteSize,
			.w = spriteSize,
			.h = spriteSize
		};

		SDL_RenderTextureRotated(state.renderer, res.idleTex, &src, &dst, 0, nullptr, flipHorizontal ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
		//swap buffers and present
		SDL_RenderPresent(state.renderer);
		prevTime = nowTime; //update the previous time
	}
	res.unload(); //clean up the texture 
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