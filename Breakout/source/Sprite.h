#pragma once

#include <glm\vec2.hpp>
#include <string>
#include <vector>

class Texture2D;

struct Sprite_Frame
{
	std::string _name;
	glm::vec2 _size_of_sprite;
	glm::vec2 _position_in_texture; //in pixels

	Sprite_Frame(std::string name, glm::vec2 size, glm::vec2 pos)
		: _name(name), _size_of_sprite(size), _position_in_texture(pos)
	{}
};

struct Sprite
{
	Sprite() {}
	Sprite(Texture2D* sprite_sheet, glm::vec2 size, const char* frame_data) : _sprite_sheet(sprite_sheet), _size_of_sprite(size) {};
	Texture2D* _sprite_sheet;
	glm::vec2 _size_of_sprite;
	glm::vec2 _position_in_texture;
	unsigned int _current_frame;
	std::vector<Sprite_Frame> _frames;

	glm::vec2 get_frame_position()
	{
		return glm::vec2(
			_position_in_texture.x / _sprite_sheet->_width,
			_position_in_texture.y / _sprite_sheet->_height);
	}

	glm::vec2 get_frame_size()
	{
		return glm::vec2(
			_size_of_sprite.x / _sprite_sheet->_width, 
			_size_of_sprite.y / _sprite_sheet->_height);
	}
};