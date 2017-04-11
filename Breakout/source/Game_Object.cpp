#include "Game_Object.h"

Game_Object::Game_Object(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity)
	: _position(pos), _velocity(velocity), _size(size), _color(color), _rotation(0), _visable(true), _sprite(sprite)
{
}

Game_Object::Game_Object()
	: _position(glm::vec2(0.0f)), _velocity(glm::vec2(0.0f)), _size(glm::vec2(1.0f)), _color(glm::vec3(1.0f)), _rotation(0), _visable(true), _sprite()
{
}

Game_Object::~Game_Object()
{
}

void Game_Object::Draw(Sprite_Manager & render)
{
	render.DrawSprite(this->_sprite, this->_position, this->_size, this->_rotation, this->_color);
}
