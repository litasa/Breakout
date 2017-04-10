#pragma once

#include <glm\vec2.hpp>
#include <string>
#include <vector>

class Texture2D;

struct Sprite_Frame
{
	std::string _id;
	glm::vec2 _size_of_sprite;
	glm::vec2 _position_in_texture;
	Texture2D* _texture;

	Sprite_Frame(std::string id, glm::vec2 size, glm::vec2 pos, Texture2D* texture)
		: _id(id), _size_of_sprite(size), _position_in_texture(pos), _texture(texture) 
	{}
};

struct Sprite
{
	Texture2D _sprite_sheet;
	glm::vec2 _size_of_sprite;
	glm::vec2 _position_in_texture; //in pixels

	glm::vec2 get_frame_position()
	{
		return glm::vec2(
			_position_in_texture.x / _sprite_sheet._width,
			_position_in_texture.y / _sprite_sheet._height);
	}

	glm::vec2 get_frame_size()
	{
		return glm::vec2(
			_size_of_sprite.x / _sprite_sheet._width, 
			_size_of_sprite.y / _sprite_sheet._height);
	}
};