#include "Ball_Object.h"

Ball_Object::Ball_Object(glm::vec2 pos, float radius, glm::vec2 velocity, Sprite* sprite)
	: Game_Object(pos, glm::vec2(2*radius), sprite, glm::vec3(1), velocity), _radius(radius), _stuck_to_paddle(true), _sprite(sprite)
{
}

Ball_Object::Ball_Object()
	: Game_Object(), _radius(0)
{
}

glm::vec2 Ball_Object::Move(float dt, unsigned int window_width)
{
	if (!this->_stuck_to_paddle)
	{
		this->_position += this->_velocity *dt;

		if (this->_position.x <= 0.0f)
		{
			this->_velocity.x = -this->_velocity.x;
			this->_position.x = 0;
		}
		else if (this->_position.x + this->_size.x >= window_width)
		{
			this->_velocity.x = -this->_velocity.x;
			this->_position.x = window_width - this->_size.x;
		}
		if (this->_position.y <= 0.0f)
		{
			this->_velocity.y = -this->_velocity.y;
			this->_position.y = 0;
		}
	}
	return this->_position;
}

void Ball_Object::Reset(glm::vec2 position, glm::vec2 velocity)
{
	this->_position = position;
	this->_velocity = velocity;
	this->_stuck_to_paddle = true;
}

void Ball_Object::Draw(Sprite_Manager & render)
{
	render.DrawAnimatedSprite(this->_sprite, this->_position, this->_size, this->_rotation, this->_color);
}
