#include "LooperChannel.h"
#include <stdio.h>
#include <string>

LooperChannel::LooperChannel() :_system(nullptr), _sound(nullptr), _channel(nullptr) {}

LooperChannel::LooperChannel(FMOD::System * syst, const short & channelNo): _system(syst), _channelNo(channelNo)
{
	_attr.loop = false;
	_attr.pitch = 1.0f;
	_attr.volume = 1.0f;

	rect.x = 0;
	rect.y = 0;
	rect.w = 100;
	rect.h = 600;

	//Efectos	
	_system->createDSPByType(FMOD_DSP_TYPE_ECHO, &dsp_echo);	
	dsp_echo->setParameterFloat(FMOD_DSP_ECHO_DELAY, echo);
	_system->createDSPByType(FMOD_DSP_TYPE_FLANGE, &dsp_flange);	
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
	Carga un sonido desde una ruta del ordenador.
	@param soundName: nombre del sonido, almacenado en la carpeta Sounds del proyecto.
	@param looping: true si quieres que el sonido se reproduzca en loop infinito. false si no

*/

void LooperChannel::loadFile(const char * path, const bool & looping)
{
	std::string buf(path);
	//buf.append(soundName);
	FMOD_RESULT result = _system->createSound(buf.data(), NULL, 0, &_sound);
	
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

void LooperChannel::pauseSound()
{
	bool aux;
	_channel->getPaused(&aux);

	aux = !aux;
	_channel->setPaused(aux);
}

void LooperChannel::stopSound()
{
	_channel->stop();
}

bool LooperChannel::isPlaying()
{
	bool playing = true;
	if (_channel != nullptr)
		_channel->getPaused(&playing);	

	return !playing;
}

short LooperChannel::getChannelNumber()
{
	return _channelNo;
}

float LooperChannel::getPitch()
{
	_channel->getPitch(&_attr.pitch);
	return _attr.pitch;
}

void LooperChannel::setPitch(const float & p)
{
	if(p >= 0)
		_attr.pitch = p;

	setChannelAttributes(_channel);
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

float LooperChannel::getVolume()
{
	float vol = 0.0f;
	if(isPlaying())
	_channel->getVolume(&vol);

	return vol;
}

void LooperChannel::setVolume(const float & v)
{
	if (v >= -0.001 && v <= 1){
		_attr.volume = v;
		_channel->setVolume(v);
	}
}

void LooperChannel::toggleEcho()
{
	bool active = false;
	dsp_echo->getActive(&active);	

	if (active)	
		_channel->removeDSP(dsp_echo);		
	else
		_channel->addDSP(1, dsp_echo); //1 para posibilidad de repetir		
	
}

void LooperChannel::setEcho(float n)
{	
	echo = n;
	dsp_echo->setParameterFloat(FMOD_DSP_ECHO_DELAY, echo);
}

void LooperChannel::toggleFlange()
{
	bool active;

	dsp_flange->getActive(&active);

	if (active)
		_channel->removeDSP(dsp_flange);
	else	
		_channel->addDSP(1, dsp_flange);		
	
}

void LooperChannel::setChannelAttributes(FMOD::Channel * ch)
{
	ch->setVolume(_attr.volume);
	ch->setPitch(_attr.pitch);
	setLooping(_attr.loop);
}
