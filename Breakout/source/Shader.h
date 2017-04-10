#pragma once
#include <GL\glew.h>
#include <glm\vec2.hpp>
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>
#include <glm\mat4x4.hpp>
#include <string>
#include <sstream>

class Shader
{
public:
	Shader();
	~Shader();

	//State
	GLuint ID;

	Shader &Use();
	void Compile(const char* vertex_source, const char* fragment_source, const char* geometry_source, std::stringstream& ss);

	void setFloat(const char* variable, float value);
	void setInteger(const char* variable, int value);

	void setVec2(const char* variable, glm::vec2& vec);
	void setVec2(const char* variable, float x, float y);

	void setVec3(const char* variable, glm::vec3& vec);
	void setVec3(const char* variable, float x, float y, float z);

	void setVec4(const char* variable, glm::vec4& vec);
	void setVec4(const char* variable, float x, float y, float z, float w);

	void setMatrix4(const char* variable, const glm::mat4& mat);

private:
	enum class CHECK
	{
		VERTEX,
		FRAGMENT,
		GEOMETRY,
		PROGRAM
	};
	void checkCompileErrors(GLuint object, CHECK type, std::stringstream &ss);
};

