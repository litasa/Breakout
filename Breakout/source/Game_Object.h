#pragma once

#include <glm\vec2.hpp>
#include "Texture2D.h"
#include "Sprite_Renderer.h"
class Game_Object
{
public:
	Game_Object(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f));
	Game_Object();
	~Game_Object();

	glm::vec2 _position;
	glm::vec2 _velocity;
	glm::vec2 _size;

	glm::vec3 _color;

	float _rotation;
	bool _visable;

	Texture2D _sprite;

	virtual void Draw(Sprite_Renderer &render);
};