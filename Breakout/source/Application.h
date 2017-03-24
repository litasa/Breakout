#pragma once
#include "Sprite_Renderer.h"

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

private:
	Sprite_Renderer* _sprite_renderer;
};

