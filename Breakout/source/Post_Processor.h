#pragma once

#include "Shader.h"
#include "Texture2D.h"


class Post_Processor
{
public:
	Shader _shader;
	Texture2D _texture;

	unsigned int _width, _height;

	bool _confuse, _chaos, _shake;

	Post_Processor(Shader shader, unsigned int width, unsigned int height);

	//Prepares framebuffer
	void BeginRender();

	//stores data into frame buffer
	void EndRender();

	void Render(float time);

private:
	//render states
	GLuint MSFBO; //multi sample fbo
	GLuint FBO;

	GLuint RBO; //multi sample color buffer
	GLuint VAO;

	void initRenderData();
};