#pragma once

#include <glm\vec2.hpp>
#include "Texture2D.h"
#include "Sprite_Manager.h"
class Game_Object
{
public:
	Game_Object(glm::vec2 pos, glm::vec2 size, Sprite* sprite,
		glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f));
	Game_Object();
	~Game_Object();

	glm::vec2 _position;
	glm::vec2 _velocity;
	glm::vec2 _size;

	glm::vec3 _color;

	float _rotation;
	bool _visable;

	Sprite* _sprite;

	virtual void Draw(Sprite_Manager &render);
};