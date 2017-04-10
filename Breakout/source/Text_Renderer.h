#pragma once

#include <map>
#include <GL\glew.h>

#include <glm\glm.hpp>

#include "Texture2D.h"
#include "Shader.h"

struct Character {
	GLuint _texture_ID;
	glm::ivec2 _size;
	glm::ivec2 _bearing; //offset from base line to left/top of glyph
	GLuint _advance; //horizontal offset to advance to next glyph
};

class Text_Renderer
{
public:
	std::map<GLchar, Character> _characters;

	Shader _text_shader;

	Text_Renderer(GLuint width, GLuint height);

	void Load(std::string font, GLuint font_size);

	void RenderText(std::string text, float x, float y, float scale = 1.0f, glm::vec3 color = glm::vec3(1.0f));

private:
	GLuint VAO;
	GLuint VBO;
};