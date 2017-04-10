#include "Post_Processor.h"
#include <iostream>

Post_Processor::Post_Processor(Shader shader, unsigned int width, unsigned int height)
	:
	_shader(shader), _texture(Texture2D()), _width(width), _height(height),
	_confuse(false), _chaos(false), _shake(false)
{
	glGenFramebuffers(1, &this->MSFBO);
	glGenFramebuffers(1, &this->FBO);
	glGenRenderbuffers(1, &this->RBO);

	//initialize MSFBO
	glBindFramebuffer(GL_FRAMEBUFFER, this->MSFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, this->RBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_RGB, width, height); //allocate memory
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, this->RBO); //attach
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "ERROR::POSTPROCESSOR: Failed to initialize MSFBO" << std::endl;
	}

	//initialize FBO
	glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
	this->_texture.Generate(width, height, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->_texture.ID, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "ERROR::POSTPROCESSOR: Failed to initialize FBO" << std::endl;
	}
	glBindBuffer(GL_FRAMEBUFFER, 0);


	this->initRenderData();
	this->_shader.Use();
	this->_shader.setInteger("scene", 0);
	float offset = 1.0f / 500.0f; //some nice number at random
	float offsets[9][2] = {
		{ -offset,  offset },  // top-left
		{ 0.0f,    offset },  // top-center
		{ offset,  offset },  // top-right
		{ -offset,  0.0f },  // center-left
		{ 0.0f,    0.0f },  // center-center
		{ offset,  0.0f },  // center - right
		{ -offset, -offset },  // bottom-left
		{ 0.0f,   -offset },  // bottom-center
		{ offset, -offset }   // bottom-right    
	};

	glUniform2fv(glGetUniformLocation(this->_shader.ID, "offsets"), 9, (float*)offsets);
	int edge_kernel[9] = {
		-1, -1, -1,
		-1,  8, -1,
		-1, -1, -1
	};
	glUniform1iv(glGetUniformLocation(this->_shader.ID, "edge_kernel"), 9, edge_kernel);
	float blur_kernel[9] = {
		1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16
	};
	glUniform1fv(glGetUniformLocation(this->_shader.ID, "blur_kernel"), 9, blur_kernel);
}

void Post_Processor::BeginRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->MSFBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Post_Processor::EndRender()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, this->MSFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->FBO);
	glBlitFramebuffer(0, 0, this->_width, this->_height, 0, 0, this->_width, this->_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0); //binds both read and default
}

void Post_Processor::Render(float time)
{
	this->_shader.Use();
	this->_shader.setFloat("time", time);
	this->_shader.setInteger("confuse", this->_confuse);
	this->_shader.setInteger("chaos", this->_chaos);
	this->_shader.setInteger("shake", this->_shake);

	glActiveTexture(GL_TEXTURE0);
	this->_texture.Bind();
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void Post_Processor::initRenderData()
{
	GLuint VBO;
	GLfloat vertices[] = {
		// Pos        // Tex
		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f,  1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 1.0f,

		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		1.0f,  1.0f, 1.0f, 1.0f
	};

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
