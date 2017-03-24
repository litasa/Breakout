#pragma once

#include "Game_Object.h"

class Brick : public Game_Object
{
public:
	Brick(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity = glm::vec2(0.0f), bool solid = false, bool destroyed = false);
	Brick();
	bool _is_solid;
	bool _destroyed;
};