#pragma once
#include <GL\glew.h>
#include <glm\vec2.hpp>
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>
#include <glm\mat4x4.hpp>
#include <string>

class Shader
{
public:
	Shader();
	~Shader();

	//State
	GLuint ID;

	Shader &Use();
	void Compile(const char* vertex_source, const char* fragment_source, const char* geometry_source = nullptr);

	void setFloat(const char* variable, float value, bool use_shader = false);
	void setInteger(const char* variable, int value, bool use_shader = false);

	void setVec2(const char* variable, glm::vec2& vec, bool use_shader = false);
	void setVec2(const char* variable, float x, float y, bool use_shader = false);

	void setVec3(const char* variable, glm::vec3& vec, bool use_shader = false);
	void setVec3(const char* variable, float x, float y, float z, bool use_shader = false);

	void setVec4(const char* variable, glm::vec4& vec, bool use_shader = false);
	void setVec4(const char* variable, float x, float y, float z, float w, bool use_shader = false);

	void setMatrix4(const char* variable, const glm::mat4& mat, bool use_shader = false);

private:
	enum class CHECK
	{
		VERTEX,
		FRAGMENT,
		GEOMETRY,
		PROGRAM
	};
	void checkCompileErrors(GLuint object, CHECK type);
};

