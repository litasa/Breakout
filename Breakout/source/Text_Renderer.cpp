#include "Text_Renderer.h"

#include <glm\gtc\matrix_transform.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Resource_Manager.h"

#include <iostream>

Text_Renderer::Text_Renderer(GLuint width, GLuint height)
{
	//load shader
	this->_text_shader = Resource_Manager::GetShader("text");
	this->_text_shader.Use();
	this->_text_shader.setMatrix4("projection", glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f));
	this->_text_shader.setInteger("text", 0);
	//VAO & VBO for texture quads
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Text_Renderer::Load(std::string font, GLuint font_size)
{
	this->_characters.clear();

	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "Freetype library could not be initialized" << std::endl;
	}

	FT_Face face;
	if (FT_New_Face(ft, font.c_str(), 0, &face))
	{
		std::cout << "Failed to load font: " << font << std::endl;
	}

	FT_Set_Pixel_Sizes(face, 0, font_size);
	//disable byte-alinment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (GLubyte c = 0; c < 128; c++) 
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "Failed to load glyph" << std::endl;
			continue;
		}

		//generate texture for glyph
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		_characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void Text_Renderer::RenderText(std::string text, float x, float y, float scale, glm::vec3 color)
{
	this->_text_shader.Use();
	this->_text_shader.setVec3("textColor", color);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(this->VAO);

	for (auto c = text.begin(); c != text.end(); ++c)
	{
		Character ch = _characters[*c];

		float xpos = x + ch._bearing.x*scale;
		float ypos = y + (this->_characters['H']._bearing.y - ch._bearing.y) * scale;

		float w = ch._size.x * scale;
		float h = ch._size.y * scale;

		// Update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 0.0 },
			{ xpos,     ypos,       0.0, 0.0 },

			{ xpos,     ypos + h,   0.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 1.0 },
			{ xpos + w, ypos,       1.0, 0.0 }
		};

		glBindTexture(GL_TEXTURE_2D, ch._texture_ID);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		x += (ch._advance >> 6)* scale; // Bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D,0);
}
