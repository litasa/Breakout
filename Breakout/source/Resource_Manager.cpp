#include "Resource_Manager.h"



#include <iostream>
#include <sstream>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Sprite.h"


//instanciate static variables
std::unordered_map<std::string, Shader> Resource_Manager::Shaders;
std::unordered_map<std::string, Texture2D> Resource_Manager::Textures;
std::unordered_map<std::string, Sprite> Resource_Manager::Sprites;

Resource_Manager::~Resource_Manager()
{
}

Shader Resource_Manager::LoadShader(std::string name, const char * vertex_source, const char * fragment_source, const char * geometry_source)
{
	std::stringstream ss;
	Shaders[name] = loadShaderFromFile(vertex_source, fragment_source, geometry_source, ss);
	if (ss.rdbuf()->in_avail() != NULL)
	{
		std::cout << "loadShader " << name << " failed: " << ss.str() << std::endl;
	}
	return Shaders[name];
}

Shader& Resource_Manager::GetShader(std::string name)
{
	return Shaders[name];
}

Texture2D Resource_Manager::LoadTexture(std::string name, const char * file)
{
	std::stringstream ss;
	Textures[name] = loadTextureFromFile(file, ss);
	if (ss.rdbuf()->in_avail() != NULL)
	{
		std::cout << "loadTexture " << name << " failed: " << ss.str() << std::endl;
	}
	return Textures[name];
}

Texture2D& Resource_Manager::GetTexture(std::string name)
{
	return Textures[name];
}

Sprite Resource_Manager::LoadSprite(std::string name, std::string texture, const char * file)
{
	std::stringstream ss;
	Sprites[name] = loadSpriteInfoFromFile(texture, file, ss);
	if (ss.rdbuf()->in_avail() != NULL)
	{
		std::cout << "loadSprite " << name << " failed: " << ss.str() << std::endl;
	}
	return Sprites[name];
}

Sprite& Resource_Manager::GetSprite(std::string name)
{
	return Sprites[name];
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

Shader Resource_Manager::loadShaderFromFile(const char * vertex_source, const char * fragment_source, const char * geometry_source, std::stringstream &ss)
{
	std::ifstream vertex, fragment, geometry;
	std::string geometry_code;

	vertex.open(vertex_source, std::ios_base::in);
	if (!vertex.is_open()) {
		ss << "Error opening file: " << vertex_source << std::endl;
	}
	std::string vertex_code(std::istreambuf_iterator<char>(vertex), (std::istreambuf_iterator<char>()));
	vertex.close();

	fragment.open(fragment_source, std::ios_base::in);
	if (!fragment.is_open()) {
		ss << "Error opening file: " << fragment_source << std::endl;
	}
	std::string fragment_code(std::istreambuf_iterator<char>(fragment), (std::istreambuf_iterator<char>()));
	fragment.close();

	Shader shader;
	if (geometry_source != nullptr)
	{
		geometry.open(geometry_source, std::ios_base::in);
		if (!geometry.is_open()) {
			ss << "Error opening file: " << geometry_source << std::endl;
		}
		std::string geometry_code(std::istreambuf_iterator<char>(geometry), (std::istreambuf_iterator<char>()));
		geometry.close();
		shader.Compile(vertex_code.c_str(), fragment_code.c_str(), geometry_code.c_str(), ss);
	}
	else
	{
		shader.Compile(vertex_code.c_str(), fragment_code.c_str(), nullptr, ss);
	}

	return shader;
}

Texture2D Resource_Manager::loadTextureFromFile(const char * file, std::stringstream &ss)
{
	Texture2D texture;

	int width, height;

	unsigned char* image = stbi_load(file, &width, &height, 0, STBI_rgb_alpha);
	if (image == 0) {
		ss << "Error opening: " << file << std::endl;
	}
	texture.Generate(width, height, image);
	stbi_image_free(image);
	return texture;
}

Sprite Resource_Manager::loadSpriteInfoFromFile(std::string texture, const char * file, std::stringstream & ss)
{
	//TODO Add loading from file to get sprite frame data
	Texture2D* temp = &Textures[texture];
	return Sprite(temp, glm::vec2(temp->_width,temp->_height), nullptr);
}
