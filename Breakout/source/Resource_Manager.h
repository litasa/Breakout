#pragma once
#include <unordered_map>
#include <irrKlang.h>
#pragma comment(lib, "irrKlang.lib")
#include <GL\glew.h>
#include "Shader.h"
#include "Texture2D.h"

struct Sprite;

class Resource_Manager
{
public:
	~Resource_Manager();

	static Shader LoadShader(std::string name, const char* vertex_source, const char* fragment_source, const char* geometry_source);
	static Shader& GetShader(std::string name);

	static Texture2D LoadTexture(std::string name, const char* file);
	static Texture2D& GetTexture(std::string name);

	static Sprite LoadSprite(std::string name, std::string texture, const char* file);
	static void LoadSpritesFromFile(std::string texture, const char* file);
	static Sprite& GetSprite(std::string name);

	static void Clear();

private:
	//Can not create, all public classes should be static
	Resource_Manager() { }

	static Shader loadShaderFromFile(const char* vertex_source, const char* fragment_source, const char* geometry_source, std::stringstream &ss);

	static Texture2D loadTextureFromFile(const char* file, std::stringstream &ss);

	static Sprite loadSpriteInfoFromFile(std::string texture, const char* file, std::stringstream &ss);

	static std::unordered_map<std::string, Shader> Shaders;
	static std::unordered_map<std::string, Texture2D> Textures;
	static std::unordered_map<std::string, Sprite> Sprites;
	

};

