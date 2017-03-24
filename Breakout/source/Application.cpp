#include "Application.h"
#include "Resource_Manager.h"

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

	Resource_Manager::LoadTexture("./data/textures/awesomeface.png", "face");
}

void Application::ProcessInput(float dt)
{
}

void Application::Update(float dt)
{
}

void Application::Render()
{
	_sprite_renderer->DrawSprite(Resource_Manager::GetTexture("face"),
		glm::vec2(200, 200), glm::vec2(300, 400), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}
