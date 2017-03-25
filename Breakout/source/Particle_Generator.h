#pragma once
#include <glm\vec2.hpp>
#include <glm\vec4.hpp>

#include <vector>

#include "Shader.h"
#include "Texture2D.h"
#include "Game_Object.h"

struct Particle {
	glm::vec2 _position;
	glm::vec2 _velocity;

	glm::vec4 _color;
	float _lifetime;

	Particle()
		:
		_position(0.0f), _velocity(0.0f), _color(1.0f), _lifetime(0.0f)
	{
	}
};

class Particle_Generator
{
public:
	Particle_Generator(Shader shader, Texture2D texture, unsigned int amount);

	void Update(float dt, Game_Object &object, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0f));

	void Draw();

private:
	std::vector<Particle> _particles;
	unsigned int _number_particles;

	float _fade_amount;

	Shader _shader;
	Texture2D _texture;

	GLuint VAO;

	unsigned int _last_used_particle = 0;

	void init();

	unsigned int firstUnusedParticle();

	void respawnParticle(Particle &particle, Game_Object &object, glm::vec2 offset = glm::vec2(0.0f));
};