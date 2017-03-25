#include "Particle_Generator.h"

Particle_Generator::Particle_Generator(Shader shader, Texture2D texture, unsigned int amount)
	:
	_shader(shader), _texture(texture), _number_particles(amount)
{
	this->init();
}

void Particle_Generator::Update(float dt, Game_Object & object, unsigned int newParticles, glm::vec2 offset)
{
	for (unsigned int i = 0; i < newParticles; i++)
	{
		unsigned int unused = this->firstUnusedParticle();
		this->respawnParticle(this->_particles[unused], object, offset);
	}

	for (unsigned int i = 0; i < this->_number_particles; ++i)
	{
		Particle &p = this->_particles[i];
		p._lifetime -= dt;
		if (p._lifetime > 0.0f)
		{
			p._position -= p._velocity*dt;
			p._color.a -= dt*2.5;
		}
	}
}

void Particle_Generator::Draw()
{
	// Additive blending for glow effect
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	this->_shader.Use();

	for (Particle particle : this->_particles)
	{
		if (particle._lifetime > 0.0f)
		{
			this->_shader.setVec2("offset", particle._position);
			this->_shader.setVec4("color", particle._color);
			this->_shader.setFloat("scale", 10.0f);
			this->_texture.Bind();
			glBindVertexArray(this->VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}
	//reset glow effect
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Particle_Generator::init()
{
	// Set up mesh and attribute properties
	GLuint VBO;
	GLfloat particle_quad[] = {
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (GLvoid*)0);
	glBindVertexArray(0);

	this->_particles.reserve(this->_number_particles);
	for (unsigned int i = 0; i < this->_number_particles; ++i)
	{
		this->_particles.push_back(Particle());
	}
}

unsigned int Particle_Generator::firstUnusedParticle()
{
	//search from last_used_particle
	for (unsigned int i = this->_last_used_particle; i < this->_number_particles; ++i)
	{
		if (this->_particles[i]._lifetime <= 0.0f)
		{
			_last_used_particle = i;
			return i;
		}
	}
	//else start from the beginning
	for (unsigned int i = this->_last_used_particle; i < this->_number_particles; ++i)
	{
		if (this->_particles[i]._lifetime <= 0.0f)
		{
			_last_used_particle = i;
			return i;
		}
	}

	//overwrite first particle
	_last_used_particle = 0;
	return 0;
}

void Particle_Generator::respawnParticle(Particle & particle, Game_Object & object, glm::vec2 offset)
{
	//TODO: this information should be inside the specific particle
	GLfloat random = ((rand() % 100) - 50) / 10.0f;
	GLfloat rColor = 0.5 + ((rand() % 100) / 100.0f);
	particle._position = object._position + random + offset;
	particle._color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle._lifetime = 1.0f;
	particle._velocity = object._velocity * 0.1f;
}
