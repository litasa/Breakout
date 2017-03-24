#pragma once
#include <GL\glew.h>

class Texture2D
{
public:
	Texture2D();
	~Texture2D();

	void Generate(unsigned int width, unsigned int height, unsigned char* data);
	void Bind() const;

	GLuint ID;

	unsigned int _height;
	unsigned int _width;

	GLuint _internal_format;
	GLuint _image_format;

	GLuint wrap_s;
	GLuint wrap_t;
	GLuint filter_min;
	GLuint filter_max;
private:

};

