#include "Resource_Manager.h"



#include <iostream>
#include <fstream>
#include <SOIL.h>


//instanciate static variables
std::unordered_map<std::string, Shader> Resource_Manager::Shaders;
std::unordered_map<std::string, Texture2D> Resource_Manager::Textures;

Resource_Manager::~Resource_Manager()
{
}

Shader Resource_Manager::LoadShader(const char * vertex_source, const char * fragment_source, const char * geometry_source, std::string name)
{
	Shaders[name] = loadShaderFromFile(vertex_source, fragment_source, geometry_source);
	return Shaders[name];
}

Shader Resource_Manager::GetShader(std::string name)
{
	return Shaders[name];
}

Texture2D Resource_Manager::LoadTexture(const char * file, std::string name)
{
	Textures[name] = loadTextureFromFile(file);
	return Textures[name];
}

Texture2D Resource_Manager::GetTexture(std::string name)
{
	return Textures[name];
}

void Resource_Manager::Clear()
{
	for (auto it : Shaders) {
		glDeleteProgram(it.second.ID);
	}
	for (auto it : Textures) {
		glDeleteTextures(1, &it.second.ID);
	}
}

Shader Resource_Manager::loadShaderFromFile(const char * vertex_source, const char * fragment_source, const char * geometry_source)
{
	std::ifstream vertex, fragment, geometry;
	std::string geometry_code;

	vertex.open(vertex_source, std::ios_base::in);
	if (!vertex.is_open()) {
		std::cerr << "Error opening file: " << vertex_source << std::endl;
	}
	std::string vertex_code(std::istreambuf_iterator<char>(vertex), (std::istreambuf_iterator<char>()));
	vertex.close();

	fragment.open(fragment_source, std::ios_base::in);
	if (!fragment.is_open()) {
		std::cerr << "Error opening file: " << fragment_source << std::endl;
	}
	std::string fragment_code(std::istreambuf_iterator<char>(fragment), (std::istreambuf_iterator<char>()));
	fragment.close();

	Shader shader;
	if (geometry_source != nullptr)
	{
		geometry.open(geometry_source, std::ios_base::in);
		if (!geometry.is_open()) {
			std::cerr << "Error opening file: " << geometry_source << std::endl;
		}
		std::string geometry_code(std::istreambuf_iterator<char>(geometry), (std::istreambuf_iterator<char>()));
		geometry.close();
		shader.Compile(vertex_code.c_str(), fragment_code.c_str(), geometry_code.c_str());
	}
	else
	{
		shader.Compile(vertex_code.c_str(), fragment_code.c_str(), nullptr);
	}
	

	return shader;
}

Texture2D Resource_Manager::loadTextureFromFile(const char * file)
{
	Texture2D texture;

	int width, height;

	unsigned char* image = SOIL_load_image(file, &width, &height, 0, SOIL_LOAD_RGBA);
	if (image == 0) {
		std::cerr << "Error opening: " << file << std::endl;
	}
	texture.Generate(width, height, image);
	SOIL_free_image_data(image);
	return texture;
}
