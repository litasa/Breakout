#pragma once

#include <unordered_map>
#include <string>
#include <irrKlang.h>

class Sound_Manager
{
public:
	
	static void add_sound_source(std::string name, const char* file);
	static void play(std::string name);
	static void stop(std::string name);
	///volume between 0.0f and 1.0f
	static void set_volume(std::string name, float volume);

private:
	Sound_Manager() { };
	static irrklang::ISoundEngine* _engine;
	static std::unordered_map<std::string, irrklang::ISound*> _sounds;
	static std::unordered_map<std::string, irrklang::ISoundSource*> _sound_sources;
};