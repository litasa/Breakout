#pragma once
#include "Sprite_Renderer.h"
#include "Game_Level.h"

class Application
{
public:
	Application(unsigned int width, unsigned int height);
	~Application();

	void Init();

	void ProcessInput(float dt);
	void Update(float dt);
	void Render();

	enum class State
	{
		ACTIVE,
		MENU,
		WIN
	};

	//states
	State _state;
	bool _keys[1024];
	unsigned int _width, _height;

	unsigned int _current_level;
	std::vector<Game_Level> _levels;

	Game_Object* _player;

private:
	Sprite_Renderer* _sprite_renderer;

	const glm::vec2 PLAYER_SIZE = glm::vec2(100.0f, 20.0f);
	const float PLAYER_VELOCITY = float(500.0f);
};

