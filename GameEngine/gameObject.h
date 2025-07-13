#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "animation.h"

#include <SDL3/SDL.h>

enum class ObjectType
{
	player, level, enemy
};

struct GameObject
{
	ObjectType type; //type of the game object
	glm::vec2 position, velocity, acceleration;
	float direction;
	std::vector<Animation> animations; //animations for the game object
	int currentAnimation;
	SDL_Texture* texture;

	GameObject()
	{
		type = ObjectType::level; //default type is level
		position = glm::vec2(0.0f, 0.0f); //default position is (0, 0)
		velocity = glm::vec2(0.0f, 0.0f); //default velocity is (0, 0)
		acceleration = glm::vec2(0.0f, 0.0f); //default acceleration is (0, 0)
		direction = 1.0f; //default direction is 1 degrees
		currentAnimation = -1; //no current animation
		texture = nullptr;

	}
};