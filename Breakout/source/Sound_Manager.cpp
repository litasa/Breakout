#include "Sound_Manager.h"
#include "Resource_Manager.h"

//initialize static members
irrklang::ISoundEngine* Sound_Manager::_engine = irrklang::createIrrKlangDevice();
std::unordered_map<std::string, irrklang::ISound*> Sound_Manager::_sounds;
std::unordered_map<std::string, irrklang::ISoundSource*> Sound_Manager::_sound_sources;

void Sound_Manager::add_sound_source(std::string name, const char * file)
{
	_sound_sources[name] = _engine->addSoundSourceFromFile(file);
	_sounds[name] = _engine->play2D(_sound_sources[name], false, true, true);
}

void Sound_Manager::play(std::string name)
{
	stop(name);
	_sounds[name] = _engine->play2D(
		_sound_sources[name],
		false/*playLooped*/,
		false/*startPaused*/,
		true/*trackSound*/);
}

void Sound_Manager::stop(std::string name)
{
	irrklang::ISound* snd = _sounds[name];
	if (!snd->isFinished())
	{
		snd->stop();
	}
}

void Sound_Manager::set_volume(std::string name, float volume)
{
	_sound_sources[name]->setDefaultVolume(volume);
}
