#include "LooperChannel.h"
#include <stdio.h>
#include <string>



LooperChannel::LooperChannel() :_system(nullptr), _sound(nullptr), _channel(nullptr) {}

LooperChannel::LooperChannel(FMOD::System * syst, const short & channelNo): _system(syst), _channelNo(channelNo)
{
	_attr.loop = false;
	_attr.pitch = 1.0f;
	_attr.volume = 1.0f;
}

LooperChannel::LooperChannel(FMOD::System * syst, const char * soundName, const short & channelNo): _system(syst), _channelNo(channelNo)
{	
	_attr.loop = false;
	_attr.pitch = 1.0f;
	_attr.volume = 1.0f;
	std::string buf("../Sounds/");
	buf.append(soundName);
	_attr.loop = false;
	_attr.pitch = 1.0f;
	_attr.volume = 1.0f;

	_system->createSound(buf.data(), FMOD_LOOP_OFF, 0, &_sound);
}

LooperChannel::~LooperChannel()
{
	_sound->release();
}

/*
	Carga un sonido desde otro Sound.
	@param sound: FMOD::Sound que se asigna a LooperSound.
	@param looping: true si quieres que el sonido se reproduzca en loop infinito. false si no
*/
void LooperChannel::loadSound(FMOD::Sound * sound, const bool & looping)
{
	_sound->release();
	_sound = sound;

	setLooping(looping);

}


/*
	Carga un sonido desde la carpeta Sounds desde el proyecto.
	@param soundName: nombre del sonido, almacenado en la carpeta Sounds del proyecto.
	@param looping: true si quieres que el sonido se reproduzca en loop infinito. false si no

*/
void LooperChannel::loadSound(const char * soundName, const bool & looping)
{
	std::string buf("../Sounds/");
	buf.append(soundName);
	_system->createSound(buf.data(),NULL, 0, &_sound);
	setLooping(looping);
}


/*
	Reproduce un sonido.
	Si el sonido ya estaba en reproducción, corta la misma y vuelve a reproducirlo desde el principio.
	Si no estaba en reproducción, lo reproduce de manera normal
*/
void LooperChannel::playSound()
{
	bool isP;
	_channel->isPlaying(&isP);
	if (isP) {
		_channel->setPaused(true);
		_system->playSound(_sound, 0, false, &_channel);
	}
	else {
		_system->playSound(_sound, 0, false, &_channel);
		
	}
	setLooping(_attr.loop);
}
short LooperChannel::getChannelNumber()
{
	return _channelNo;
}

float LooperChannel::getPitch()
{
	return _attr.pitch;
}

void LooperChannel::setPitch(const float & p)
{
	if(p >= 0)
		_attr.pitch = p;
}

void LooperChannel::setLooping(const bool &lp)
{
	_attr.loop = lp;
	if (lp)
		_channel->setMode(FMOD_LOOP_NORMAL);
	else
		_channel->setMode(FMOD_LOOP_OFF);
}

bool LooperChannel::getLooping()
{
	return _attr.loop;
}

void LooperChannel::setChannelAttributes(FMOD::Channel * ch)
{
	ch->setVolume(_attr.volume);
	ch->setPitch(_attr.pitch);
	setLooping(_attr.loop);
}
