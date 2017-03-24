#pragma once
#include <unordered_map>

#include <GL\glew.h>
#include "Shader.h"
#include "Texture2D.h"

class Resource_Manager
{
public:
	~Resource_Manager();

	static std::unordered_map<std::string, Shader> Shaders;
	static std::unordered_map<std::string, Texture2D> Textures;

	static Shader LoadShader(const char* vertex_source, const char* fragment_source, const char* geometry_source, std::string name);
	static Shader GetShader(std::string name);

	static Texture2D LoadTexture(const char* file, std::string name);
	static Texture2D GetTexture(std::string name);

	static void Clear();

private:
	//Can not create, all public classes should be static
	Resource_Manager() { }

	static Shader loadShaderFromFile(const char* vertex_source, const char* fragment_source, const char* geometry_source = nullptr);

	static Texture2D loadTextureFromFile(const char* file);
};

