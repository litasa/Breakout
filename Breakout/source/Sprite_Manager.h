#pragma once

#include "Shader.h"
#include "Texture2D.h"

#include <unordered_map>

struct Sprite;

class Sprite_Manager
{
public:
	Sprite_Manager(Shader &shader);
	~Sprite_Manager();

	void DrawSprite(Texture2D &texture, glm::vec2 position,
		glm::vec2 size = glm::vec2(10, 10),
		float rotation = 0.0f,
		glm::vec3 color = glm::vec3(1.0f));

	void DrawAnimatedSprite(Sprite* sprite, glm::vec2 position, float rotation = 0.0f, glm::vec3 color = glm::vec3(1.0f));
	void Draw();

	void Update(float dt);

	void AddSprite(std::string name, Texture2D* texture, const char *frame_data);

private:
	Shader _shader;
	GLuint _quadVAO;

	std::unordered_map<std::string, Sprite> _sprites;
	void initRenderData();
};