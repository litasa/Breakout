#include "Application.h"

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
}

void Application::ProcessInput(float dt)
{
}

void Application::Update(float dt)
{
}

void Application::Render()
{
}
