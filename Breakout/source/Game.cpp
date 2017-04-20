#include "Game.h"
#include "Resource_Manager.h"

#include <GLFW\glfw3.h>

#include <glm\gtc\matrix_transform.hpp>
#include <iostream>

Game::Game(unsigned int width, unsigned int height)
	: lives(3),
	_state(State::ACTIVE),
	_width(width),
	_height(height)
{
}


Game::~Game()
{
	delete _player;
	delete _ball;
	delete _particle_generator;
	delete _sprite_manager;
	delete _special_effects;
	_sound_engine->drop(); //deletes the sound engine
	delete _text_renderer;
}

void Game::Init()
{
	//load shaders
	Resource_Manager::LoadShader("sprite", "./data/shaders/sprite.vertex", "./data/shaders/sprite.fragment", nullptr);
	Resource_Manager::LoadShader("postprocessing", "./data/shaders/effects.vertex", "./data/shaders/effects.fragment", nullptr);
	Resource_Manager::LoadShader("particle", "./data/shaders/particle.vertex", "./data/shaders/particle.fragment", nullptr);
	Resource_Manager::LoadShader("text", "./data/shaders/text.vertex", "./data/shaders/text.fragment", nullptr);

	//setup shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->_width), static_cast<float>(this->_height), 0.0f, -1.0f, 1.0f);

	Resource_Manager::GetShader("sprite").Use().setInteger("sprite_texture", 0); //sampler2D
	Resource_Manager::GetShader("sprite").setMatrix4("projection", projection);

	Resource_Manager::GetShader("particle").Use().setInteger("sprite_texture", 0); //sampler2D
	Resource_Manager::GetShader("particle").setMatrix4("projection", projection);

	Resource_Manager::GetShader("postprocessing").Use().setInteger("scene", 0); //sampler2D
	Resource_Manager::GetShader("postprocessing").setMatrix4("projection", projection);
	
	//load textures
	Resource_Manager::LoadTexture("background", "./data/textures/background.jpg");
	Resource_Manager::LoadTexture("block", "./data/textures/block.png");
	Resource_Manager::LoadTexture("block_solid", "./data/textures/block_solid.png");
	Resource_Manager::LoadTexture("particle", "./data/textures/particle.png");
	Resource_Manager::LoadTexture("paddle", "./data/textures/paddle.png");
	Resource_Manager::LoadTexture("ball", "./data/textures/awesomeface.png");
	Resource_Manager::LoadTexture("grant", "./data/textures/runningGrant.png");

	//setup extra systems
	_sprite_manager = new Sprite_Manager(Resource_Manager::GetShader("sprite"));
	_special_effects = new Post_Processor(Resource_Manager::GetShader("postprocessing"), this->_width, this->_height);
	_particle_generator = new Particle_Generator(
		Resource_Manager::GetShader("particle"),
		Resource_Manager::GetTexture("particle"),
		500);
	_sound_engine = irrklang::createIrrKlangDevice();
	_text_renderer = new Text_Renderer(this->_width, this->_height);

	//init sound engine
	if (!_sound_engine) {
		std::cerr << "Error starting up sound engine" << std::endl;
	}
	//_sound_engine->play2D("./data/sound/Helena_Jakob.wav", true); //setup looping background music Should load as levels are loaded

	//init text
	_text_renderer->Load("./data/fonts/OCRAEXT.TTF", 24);

	_player = new Game_Object(glm::vec2(0,0), PLAYER_SIZE, Resource_Manager::GetTexture("paddle"));
	_ball = new Ball_Object(glm::vec2(0,0), BALL_RADIUS, INITIAL_BALL_VELOCITY, Resource_Manager::GetTexture("ball"));
	_sprite_manager->AddSprite("grant", &Resource_Manager::GetTexture("grant"), "nullstr");
	this->ResetPlayer();

	//load levels
	unsigned int half_height = unsigned(this->_height*0.5);
	Game_Level one;   one.Load  ("./data/levels/level_1.txt", this->_width, half_height);
	Game_Level two;	  two.Load  ("./data/levels/level_2.txt", this->_width, half_height);
	Game_Level three; three.Load("./data/levels/level_3.txt", this->_width, half_height);
	Game_Level four;  four.Load ("./data/levels/level_4.txt", this->_width, half_height);
	this->_levels.push_back(one);
	this->_levels.push_back(two);
	this->_levels.push_back(three);
	this->_levels.push_back(four);
	this->_current_level = 0;
}

void Game::ProcessInput(float dt)
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


	if (this->_keys[GLFW_KEY_P] && this->_state != State::MENU)
	{
			this->_last_state = this->_state;
			this->_state = State::MENU;
	}
	else if (this->_keys[GLFW_KEY_P] && this->_state == State::MENU)
	{
		this->_state = this->_last_state;
	}
}

void Game::Update(float dt)
{
	if (_state == State::ACTIVE && this->_levels[this->_current_level].Is_Completed())
	{
		this->ResetLevel();
		this->ResetPlayer();
		this->_state = State::WIN;
	}
	if (_state == State::ACTIVE)
	{
		_ball->Move(dt, this->_width);

		this->PerformCollision();

		this->_particle_generator->Update(dt, *_ball, 2, glm::vec2(_ball->_radius / 2, _ball->_radius / 2));
		this->_sprite_manager->Update(dt);

		if (_ball->_position.y >= this->_height) // Did ball reach bottom edge?
		{
			this->ResetLevel();
			this->ResetPlayer();
			--this->lives;
			if (this->lives == 0)
			{
				this->_state = State::LOOSE;
			}
		}
		if (_shake_time > 0.0f)
		{
			_shake_time -= dt;
			if (_shake_time <= 0.0f)
			{
				_special_effects->_shake = false;
			}
		}
	}
	
}

void Game::Render()
{
	if (this->_state == State::ACTIVE)
	{
		
		_special_effects->BeginRender();
		
		_sprite_manager->DrawSprite(Resource_Manager::GetTexture("background"),
			glm::vec2(0, 0), glm::vec2(this->_width, this->_height), 0);

		this->_levels[this->_current_level].Draw(*_sprite_manager);

		this->_player->Draw(*_sprite_manager);
		this->_ball->Draw(*_sprite_manager);
		
		/*_sprite_manager->DrawSprite(Resource_Manager::GetTexture("grant"),
			glm::vec2(0, 0), glm::vec2(this->_width, this->_height), 0);*/

		//_sprite_manager->DrawAnimatedSprite(*_grant, glm::vec2(0, 0));
		//_sprite_manager->Draw();

		this->_particle_generator->Draw();

		_special_effects->EndRender();
		_special_effects->Render(float(glfwGetTime()));

		std::stringstream ss; ss << this->lives;
		_text_renderer->RenderText("Lives:" + ss.str(), 5.0f, 5.0f, 1.0f);
	}
	else if (this->_state == State::WIN)
	{
		_text_renderer->RenderText("You Won", float(_width / 2), float(_height / 2));
	}
	else if (this->_state == State::LOOSE)
	{
		_text_renderer->RenderText("You Lost", float(_width / 2), float(_height / 2));
	}
	else if (this->_state == State::MENU)
	{
		_text_renderer->RenderText("This is the meny", float(_width / 2), float(_height / 2));
	}
}

bool Game::checkCollision(Game_Object & first, Game_Object & second)
{
	//x-axis collision
	bool collision_x = first._position.x + first._size.x >= second._position.x &&
		second._position.x + second._size.x >= first._position.x;

	//y-axis collision
	bool collision_y = first._position.y + first._size.y >= second._position.y &&
		second._position.y + second._size.y >= first._position.y;

	return collision_x && collision_y;
}

Game::Collision Game::checkCollision(Game_Object & first, Ball_Object & ball)
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

void Game::ResetLevel()
{
	unsigned int half_height = unsigned(this->_height* 0.5f);
	if (this->_current_level == 0)
		this->_levels[0].Load("./data/levels/level_1.txt", this->_width, half_height);
	else if (this->_current_level == 1)
		this->_levels[1].Load("./data/levels/level_2.txt", this->_width, half_height);
	else if (this->_current_level == 2)
		this->_levels[2].Load("./data/levels/level_3.txt", this->_width, half_height);
	else if (this->_current_level == 3)
		this->_levels[3].Load("./data/levels/level_4.txt", this->_width, half_height);
}

void Game::ResetPlayer()
{
	// Reset player/ball stats
	_player->_size = PLAYER_SIZE;
	_player->_position= glm::vec2(this->_width/ 2 - PLAYER_SIZE.x / 2, this->_height- PLAYER_SIZE.y);
	_ball->Reset(_player->_position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);
}

Game::Direction Game::VectorHitDirection(glm::vec2 target)
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

void Game::PerformCollision()
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
					this->_sound_engine->play2D("./data/sound/bleep.mp3");
				}
				else
				{
					_shake_time = 0.05f;
					_special_effects->_shake = true;
					this->_sound_engine->play2D("./data/sound/solid.wav");
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
				this->_sound_engine->play2D("./data/sound/bleep.wav", false);
			}
		}
	}
}
