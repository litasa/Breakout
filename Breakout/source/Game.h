#pragma once
#include "Sprite_Manager.h"
#include "Game_Level.h"
#include "Ball_Object.h"
#include "Particle_Generator.h"
#include "Post_Processor.h"
#include <irrKlang.h>
#include "Text_Renderer.h"

#include "Sprite.h"

#include <ft2build.h>
#include FT_FREETYPE_H

class Game
{
	enum Direction
	{
		UP,
		DOWN,
		RIGHT,
		LEFT
	};

	typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;
public:
	Game(unsigned int width, unsigned int height);
	~Game();

	void Init();

	void ProcessInput(float dt);
	void Update(float dt);
	void Render();

	enum class State
	{
		ACTIVE,
		MENU,
		WIN,
		LOOSE
	};

	//states
	State _state;
	bool _keys[1024];
	unsigned int _width, _height;

	unsigned int _current_level;
	std::vector<Game_Level> _levels;

	Game_Object* _player;
	Ball_Object* _ball;
	Sprite* _grant;
	
	int lives;

private:
	Sprite_Manager* _sprite_manager;
	Post_Processor* _special_effects;
	Particle_Generator* _particle_generator;
	irrklang::ISoundEngine *_sound_engine;
	Text_Renderer* _text_renderer;

	const glm::vec2 PLAYER_SIZE = glm::vec2(100.0f, 20.0f);
	const float PLAYER_VELOCITY = float(500.0f);

	// Initial velocity of the Ball
	const glm::vec2 INITIAL_BALL_VELOCITY = glm::vec2(100.0f, -350.0f);
	const float BALL_RADIUS = 12.5f;

	// AABB - AABB collision
	bool checkCollision(Game_Object &first, Game_Object &second);

	//AABB - Circle collision
	Collision checkCollision(Game_Object &first, Ball_Object &ball);

	void ResetLevel();
	void ResetPlayer();
	

	Direction VectorHitDirection(glm::vec2 target);

	void PerformCollision();

	float _shake_time = 0;
};

