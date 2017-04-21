#include "Game_Level.h"

#include "Resource_Manager.h"
#include <fstream>
#include <sstream>
Game_Level::Game_Level()
{
}
Game_Level::~Game_Level()
{
}
void Game_Level::Load(const char * file, unsigned int level_width, unsigned int level_height)
{
	this->_bricks.clear();

	unsigned int tile_code;
	std::string line;
	std::ifstream fstream(file);
	std::vector<std::vector<unsigned int>> tile_data;

	if (fstream)
	{
		while (std::getline(fstream, line)) //read each line
		{
			std::istringstream stream(line);
			std::vector<unsigned int> row;

			while (stream >> tile_code) //each number in line
			{
				row.push_back(tile_code);
			}
			tile_data.push_back(row);
		}
		if (tile_data.size() > 0)
		{
			this->initialize(tile_data, level_width, level_height);
		}
	}
}

void Game_Level::Draw(Sprite_Manager & renderer)
{
	for (Brick &tile : this->_bricks)
	{
		if (!tile._destroyed)
		{
			tile.Draw(renderer);
		}
	}
}

bool Game_Level::Is_Completed()
{
	for (Brick &tile : _bricks)
	{
		if (!tile._is_solid && !tile._destroyed)
		{
			return false;
		}
	}
	return true;
}

void Game_Level::initialize(std::vector<std::vector<unsigned int>> tile_data, unsigned int level_width, unsigned int level_height)
{
	size_t width = tile_data[0].size();
	size_t height = tile_data.size();

	float unit_width = level_width / static_cast<float>(width);
	float unit_height = level_height / static_cast<float>(height);

	for (unsigned int y = 0; y < height; ++y)
	{
		for (unsigned int x = 0; x < width; ++x)
		{
			if (tile_data[y][x] == 1)
			{
				glm::vec2 pos = glm::vec2(unit_width*x, unit_height*y);
				glm::vec2 size = glm::vec2(unit_width, unit_height);

				Brick brick(pos, size, &Resource_Manager::GetSprite("block_solid"),
					glm::vec3(0.8f, 0.8f, 0.7f));
				brick._is_solid = true;
				this->_bricks.push_back(brick);
			}
			else if (tile_data[y][x] > 1)
			{
				glm::vec3 color = glm::vec3(1);
				if (tile_data[y][x] == 2)
					color = glm::vec3(0.2f, 0.6f, 1.0f);
				else if (tile_data[y][x] == 3)
					color = glm::vec3(0.0f, 0.7f, 0.0f);
				else if (tile_data[y][x] == 4)
					color = glm::vec3(0.8f, 0.8f, 0.4f);
				else if (tile_data[y][x] == 5)
					color = glm::vec3(1.0f, 0.5f, 0.0f);

				glm::vec2 pos(unit_width*x, unit_height*y);
				glm::vec2 size(unit_width, unit_height);

				this->_bricks.push_back(Brick(pos, size, &Resource_Manager::GetSprite("block"), color));
			}
		}
	}
}
