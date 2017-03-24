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

	//Paddle related
	Resource_Manager::LoadTexture("./data/textures/paddle.png", "paddle");
	glm::vec2 playerPos = glm::vec2(
		(this->_width - PLAYER_SIZE.x) / 2,
		this->_height - PLAYER_SIZE.y
	);
	_player = new Game_Object(playerPos, PLAYER_SIZE, Resource_Manager::GetTexture("paddle"));
	
	Resource_Manager::LoadTexture("./data/textures/awesomeface.png", "face");
	glm::vec2 ball_pos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
	_ball = new Ball_Object(ball_pos, BALL_RADIUS, INITIAL_BALL_VELOCITY, Resource_Manager::GetTexture("face"));

	//brick textures
	Resource_Manager::LoadTexture("./data/textures/background.jpg", "background");
	Resource_Manager::LoadTexture("./data/textures/block.png", "block");
	Resource_Manager::LoadTexture("./data/textures/block_solid.png", "block_solid");

	//load levels
	Game_Level one;   one.Load  ("./data/levels/level_1.txt", this->_width, this->_height*0.5);
	Game_Level two;	  two.Load  ("./data/levels/level_2.txt", this->_width, this->_height*0.5);
	Game_Level three; three.Load("./data/levels/level_3.txt", this->_width, this->_height*0.5);
	Game_Level four;  four.Load ("./data/levels/level_4.txt", this->_width, this->_height*0.5);
	this->_levels.push_back(one);
	this->_levels.push_back(two);
	this->_levels.push_back(three);
	this->_levels.push_back(four);
	this->_current_level = 1;
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
				if (_ball->_stuck_to_paddle)
				{
					_ball->_position.x -= velocity;
				}
			}
		}
		if (this->_keys[GLFW_KEY_D])
		{
			if (_player->_position.x <= this->_width - _player->_size.x)
			{
				_player->_position.x += velocity;
				if (_ball->_stuck_to_paddle)
				{
					_ball->_position.x += velocity;
				}
			}
		}


		if (this->_keys[GLFW_KEY_SPACE])
		{
			_ball->_stuck_to_paddle = false;
		}
	}
	else if (this->_state == State::MENU)
	{
		if (this->_keys[GLFW_KEY_SPACE])
		{
			this->_state = State::ACTIVE;
		}
	}
}

void Application::Update(float dt)
{
	_ball->Move(dt, this->_width);

	this->PerformCollision();
	if (_ball->_position.y >= this->_height) // Did ball reach bottom edge?
	{
		this->ResetLevel();
		this->ResetPlayer();
		this->_state = State::MENU;
	}
}

void Application::Render()
{
	if (this->_state == State::ACTIVE)
	{
		_sprite_renderer->DrawSprite(Resource_Manager::GetTexture("background"),
			glm::vec2(0, 0), glm::vec2(this->_width, this->_height), 0);

		this->_levels[this->_current_level].Draw(*_sprite_renderer);

		this->_player->Draw(*_sprite_renderer);
		this->_ball->Draw(*_sprite_renderer);
	}
}

bool Application::checkCollision(Game_Object & first, Game_Object & second)
{
	//x-axis collision
	bool collision_x = first._position.x + first._size.x >= second._position.x &&
		second._position.x + second._size.x >= first._position.x;

	//y-axis collision
	bool collision_y = first._position.y + first._size.y >= second._position.y &&
		second._position.y + second._size.y >= first._position.y;

	return collision_x && collision_y;
}

Application::Collision Application::checkCollision(Game_Object & first, Ball_Object & ball)
{
	glm::vec2 ball_center(ball._position + ball._radius);
	glm::vec2 aabb_half_extents(first._size.x / 2, first._size.y / 2);
	glm::vec2 aabb_center(
		first._position.x + aabb_half_extents.x,
		first._position.y + aabb_half_extents.y
	);

	glm::vec2 difference = ball_center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);

	glm::vec2 closest = aabb_center + clamped;

	difference = closest - ball_center;
	if (glm::length(difference) < ball._radius)
	{
		return std::make_tuple(true, VectorHitDirection(difference), difference);
	}
	return std::make_tuple(false, UP, glm::vec2(0, 0));
}

void Application::ResetLevel()
{
	if (this->_current_level == 0)
		this->_levels[0].Load("./data/levels/level_1.txt", this->_width, this->_height* 0.5f);
	else if (this->_current_level == 1)
		this->_levels[1].Load("./data/levels/level_2.txt", this->_width, this->_height* 0.5f);
	else if (this->_current_level == 2)
		this->_levels[2].Load("./data/levels/level_3.txt", this->_width, this->_height* 0.5f);
	else if (this->_current_level == 3)
		this->_levels[3].Load("./data/levels/level_4.txt", this->_width, this->_height* 0.5f);
}

void Application::ResetPlayer()
{
	// Reset player/ball stats
	_player->_size = PLAYER_SIZE;
	_player->_position= glm::vec2(this->_width/ 2 - PLAYER_SIZE.x / 2, this->_height- PLAYER_SIZE.y);
	_ball->Reset(_player->_position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);
}

Application::Direction Application::VectorHitDirection(glm::vec2 target)
{
	glm::vec2 compass[] = {
		glm::vec2(0.0f,  1.0f), //up
		glm::vec2(0.0f, -1.0f), //down
		glm::vec2(1.0f,  0.0f), //right
		glm::vec2(-1.0f,  0.0f), //left
	};
	float max = 0.0f;
	unsigned int best_match = -1;

	for (unsigned int i = 0; i < 4; ++i)
	{
		float dot_product = glm::dot(glm::normalize(target), compass[i]);
		if (dot_product > max) {
			max = dot_product;
			best_match = i;
		}
	}
	return (Direction)best_match;
}

void Application::PerformCollision()
{
	for (Brick &brick : this->_levels[this->_current_level]._bricks)
	{
		if (!brick._destroyed)
		{
			Collision collision = checkCollision(brick, *_ball);
			if (std::get<0>(collision))
			{
				if (!brick._is_solid)
				{
					brick._destroyed = true;
				}
				Direction dir = std::get<1>(collision);
				glm::vec2 diff = std::get<2>(collision);

				if (dir == LEFT || dir == RIGHT)
				{
					_ball->_velocity.x = -_ball->_velocity.x;

					float amount_penetrated = _ball->_radius - std::abs(diff.x);
					if (dir == LEFT)
					{
						_ball->_position.x += amount_penetrated;
					}
					else
					{
						_ball->_position.x -= amount_penetrated;
					}
				}
				else
				{
					_ball->_velocity.y = -_ball->_velocity.y;

					float amount_penetrated = _ball->_radius - std::abs(diff.y);
					if (dir == UP)
					{
						_ball->_position.y -= amount_penetrated;
					}
					else
					{
						_ball->_position.y += amount_penetrated;
					}
				}
			}

			collision = checkCollision(*_player, *_ball);
			if (!_ball->_stuck_to_paddle && std::get<0>(collision))
			{
				float center = _player->_position.x + _player->_size.x / 2;
				float distance_to_center = (_ball->_position.x + _ball->_radius) - center;
				float percentage = distance_to_center / (_player->_size.x / 2);

				float strength = 2.0f;
				glm::vec2 old_velocity = _ball->_velocity;
				_ball->_velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
				_ball->_velocity.y = -_ball->_velocity.y;
				_ball->_velocity = glm::normalize(_ball->_velocity) * glm::length(old_velocity);

				_ball->_velocity.y = -1 * abs(_ball->_velocity.y);
			}
		}
	}
}
