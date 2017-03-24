#pragma once
#include "Brick.h"
#include <vector>

class Game_Level
{
public:
	Game_Level();
	~Game_Level();

	void Load(const char* file, unsigned int level_width, unsigned int level_height);

	void Draw(Sprite_Renderer &renderer);

	bool Is_Completed();

	std::vector<Brick> _bricks;
private:

	
	void initialize(std::vector<std::vector<unsigned int>> tile_data, unsigned int level_width, unsigned int level_height);

};