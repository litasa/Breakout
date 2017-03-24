#include "Application.h"
#include "Resource_Manager.h"

#include <GLFW\glfw3.h>

#include <glm\gtc\matrix_transform.hpp>

Application::Application(unsigned int width, unsigned int height)
	:
	_state(State::ACTIVE),
	_width(width),
	_height(height)
{
}


Application::~Application()
{
}

void Application::Init()
{
	//load shaders
	Resource_Manager::LoadShader("./data/shaders/sprite.vertex", "./data/shaders/sprite.fragment", nullptr, "sprite");

	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->_width), static_cast<float>(this->_height), 0.0f, -1.0f, 1.0f);
	Resource_Manager::GetShader("sprite").Use().setInteger("image", 0);
	Resource_Manager::GetShader("sprite").setMatrix4("projection", projection);

	_sprite_renderer = new Sprite_Renderer(Resource_Manager::GetShader("sprite"));

	Resource_Manager::LoadTexture("./data/textures/paddle.png", "paddle");
	glm::vec2 playerPos = glm::vec2(
		(this->_width - PLAYER_SIZE.x) / 2,
		this->_height - PLAYER_SIZE.y
	);
	_player = new Game_Object(playerPos, PLAYER_SIZE, Resource_Manager::GetTexture("paddle"));

	Resource_Manager::LoadTexture("./data/textures/background.jpg", "background");
	Resource_Manager::LoadTexture("./data/textures/block.png", "block");
	Resource_Manager::LoadTexture("./data/textures/block_solid.png", "block_solid");

	Game_Level one;   one.Load  ("./data/levels/level_1.txt", this->_width, this->_height*0.5);
	Game_Level two;	  two.Load  ("./data/levels/level_2.txt", this->_width, this->_height*0.5);
	Game_Level three; three.Load("./data/levels/level_3.txt", this->_width, this->_height*0.5);
	Game_Level four;  four.Load ("./data/levels/level_4.txt", this->_width, this->_height*0.5);
	this->_levels.push_back(one);
	this->_levels.push_back(two);
	this->_levels.push_back(three);
	this->_levels.push_back(four);
	this->_current_level = 0;
}

void Application::ProcessInput(float dt)
{
	if (this->_state == State::ACTIVE)
	{
		float velocity = PLAYER_VELOCITY*dt;

		if (this->_keys[GLFW_KEY_A])
		{
			if (_player->_position.x >= 0)
			{
				_player->_position.x -= velocity;
			}
		}
		if (this->_keys[GLFW_KEY_D])
		{
			if (_player->_position.x <= this->_width - _player->_size.x)
			{
				_player->_position.x += velocity;
			}
		}
	}
}

void Application::Update(float dt)
{
}

void Application::Render()
{
	if (this->_state == State::ACTIVE)
	{
		_sprite_renderer->DrawSprite(Resource_Manager::GetTexture("background"),
			glm::vec2(0, 0), glm::vec2(this->_width, this->_height), 0);

		this->_levels[this->_current_level].Draw(*_sprite_renderer);

		this->_player->Draw(*_sprite_renderer);
	}
	
}
