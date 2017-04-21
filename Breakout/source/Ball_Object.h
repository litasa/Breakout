#pragma once
#include "Game_Object.h"


class Ball_Object : public Game_Object
{
public:
	float _radius;
	bool _stuck_to_paddle;

	Ball_Object(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite, Sprite* real_sprite);
	Ball_Object();

	glm::vec2 Move(float dt, unsigned int window_width);
	void Reset(glm::vec2 position, glm::vec2 velocity);

	virtual void Draw(Sprite_Manager &render) override;

	Sprite* _real_sprite;
};