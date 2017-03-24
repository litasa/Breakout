#include "Sprite_Renderer.h"
#include <glm\gtc\matrix_transform.hpp>

Sprite_Renderer::Sprite_Renderer(Shader & shader)
{
	this->_shader = shader;
	this->initRenderData();
}

Sprite_Renderer::~Sprite_Renderer()
{
	glDeleteVertexArrays(1, &this->_quadVAO);
}

void Sprite_Renderer::DrawSprite(Texture2D & texture, glm::vec2 position, glm::vec2 size, float rotation, glm::vec3 color)
{

	this->_shader.Use();
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(position, 0.0f));
	//top left corner is origo so move half of the quad to rotate around
	//origo
	model = glm::translate(model, glm::vec3(0.5f*size.x, 0.5f*size.y, 0.0f));
	model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f*size.x, -0.5f*size.y, 0.0f));

	model = glm::scale(model, glm::vec3(size, 1.0f));

	this->_shader.setMatrix4("model", model);
	this->_shader.setVec3("spriteColor", color);

	glActiveTexture(GL_TEXTURE0);
	texture.Bind();

	glBindVertexArray(this->_quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void Sprite_Renderer::initRenderData()
{
	GLuint VBO;
	//first two position, next two tex
	GLfloat vertices[] =
	{
		// Pos      // Tex
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &this->_quadVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->_quadVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
