#include "Texture2D.h"



Texture2D::Texture2D()
	:
	_width(0), _height(0),
	_internal_format(GL_RGBA), _image_format(GL_RGBA),
	wrap_s(GL_REPEAT), wrap_t(GL_REPEAT),
	filter_max(GL_LINEAR), filter_min(GL_LINEAR)
{
	glGenTextures(1, &this->ID);
}

Texture2D::~Texture2D()
{
}

void Texture2D::Generate(unsigned int width, unsigned int height, unsigned char* data)
{
	this->_width = width;
	this->_height = height;

	glBindTexture(GL_TEXTURE_2D, this->ID);
	glTexImage2D(GL_TEXTURE_2D, 0, this->_internal_format, width, height, 0, this->_image_format, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrap_t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filter_min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filter_max);

	glBindTexture(GL_TEXTURE_2D,0);
}

void Texture2D::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->ID);
}