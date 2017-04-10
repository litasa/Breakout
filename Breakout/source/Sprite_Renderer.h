#pragma once

#include "Shader.h"
#include "Texture2D.h"

class Sprite;

class Sprite_Renderer
{
public:
	Sprite_Renderer(Shader &shader);
	~Sprite_Renderer();

	void DrawSprite(Texture2D &texture, glm::vec2 position,
		glm::vec2 size = glm::vec2(10, 10),
		float rotation = 0.0f,
		glm::vec3 color = glm::vec3(1.0f));

	void DrawAnimatedSprite(Sprite& sprite, glm::vec2 position, float rotation = 0.0f, glm::vec3 color = glm::vec3(1.0f));

private:
	Shader _shader;
	GLuint _quadVAO;

	void initRenderData();
};