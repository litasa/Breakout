#include "Shader.h"

#include <glm\gtc\type_ptr.hpp>
#include <iostream>

Shader::Shader()
{
}


Shader::~Shader()
{
}

Shader & Shader::Use()
{
	glUseProgram(this->ID);
	return *this;
}

void Shader::Compile(const char * vertex_source, const char * fragment_source, const char * geometry_source)
{
	GLuint vertex, fragment, geometry;

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertex_source, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, CHECK::VERTEX);

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragment_source, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, CHECK::FRAGMENT);

	if (geometry_source != nullptr)
	{
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &geometry_source, NULL);
		glCompileShader(geometry);
		checkCompileErrors(geometry, CHECK::GEOMETRY);
	}
	
	this->ID = glCreateProgram();
	glAttachShader(this->ID, vertex);
	glAttachShader(this->ID, fragment);
	if (geometry_source != nullptr)
	{
		glAttachShader(this->ID, geometry);
	}
	checkCompileErrors(this->ID, CHECK::PROGRAM);

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometry_source != nullptr)
	{
		glDeleteShader(geometry);
	}
}

void Shader::setFloat(const char * variable, float value, bool use_shader)
{
	glUniform1f(glGetUniformLocation(this->ID, variable), value);
}

void Shader::setInteger(const char * variable, int value, bool use_shader)
{
	glUniform1i(glGetUniformLocation(this->ID, variable), value);

}

void Shader::setVec2(const char * variable, glm::vec2 & vec, bool use_shader)
{
	glUniform2f(glGetUniformLocation(this->ID, variable), vec.x, vec.y);

}

void Shader::setVec2(const char * variable, float x, float y, bool use_shader)
{
	glUniform2f(glGetUniformLocation(this->ID, variable), x, y);

}

void Shader::setVec3(const char * variable, glm::vec3 & vec, bool use_shader)
{
	glUniform3f(glGetUniformLocation(this->ID, variable), vec.x, vec.y, vec.z);

}

void Shader::setVec3(const char * variable, float x, float y, float z, bool use_shader)
{
	glUniform3f(glGetUniformLocation(this->ID, variable), x, y, z);

}

void Shader::setVec4(const char * variable, glm::vec4 & vec, bool use_shader)
{
	glUniform4f(glGetUniformLocation(this->ID, variable), vec.x, vec.y, vec.z, vec.w);

}

void Shader::setVec4(const char * variable, float x, float y, float z, float w, bool use_shader)
{
	glUniform4f(glGetUniformLocation(this->ID, variable), x, y, z, w);

}

void Shader::setMatrix4(const char * variable, const glm::mat4 & mat, bool use_shader)
{
	glUniformMatrix4fv(glGetUniformLocation(this->ID, variable), 1, GL_FALSE, glm::value_ptr(mat));

}

void Shader::checkCompileErrors(GLuint object, CHECK type)
{
	int success;
	GLchar info_log[1024];

	if (type != CHECK::PROGRAM)
	{
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(object, 1024, NULL, info_log);
			std::cout << "| ERROR::Shader: Compile-time error: Type: " << (int)type << "\n"
				<< info_log << "\n---------" << std::endl;
		}
	}
	else
	{
		glGetProgramiv(object, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(object, 1024, NULL, info_log);
			std::cout << "| ERROR::Shader: Link-time error: Type: " << (int)type << "\n"
				<< info_log << "\n----------" << std::endl;
		}
	}
}
