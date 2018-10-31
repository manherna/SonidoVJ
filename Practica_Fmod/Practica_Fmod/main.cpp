#include <fmod.hpp>
#include <fmod_errors.h>
#include <iostream>
#include <thread>


using namespace FMOD;
System * syst;

void ERRCHECK(FMOD_RESULT result)
{
	if (result != FMOD_OK) {
		std::cout << FMOD_ErrorString(result) << std::endl;
		exit(-1);
	}

}

class PotatoSOUND
{	
public:

	PotatoSOUND(const char * path) 
	{
		FMOD_RESULT res;
		res = syst->createSound(path, FMOD_DEFAULT, 0, &_sound);
		ERRCHECK(res);
	

	}
	~PotatoSOUND()
	{
		_sound->release();
		delete _channel;
	}
	void PlaySound()
	{
		FMOD_RESULT res = syst->playSound(_sound, 0, false, &_channel);
	}
	void togglePause()
	{	
		bool paused;
		_channel->getPaused(&paused);
		_channel->setPaused(!paused);	

	}
	void fadeIn() {
		unsigned long long parentclock;
		FMOD_RESULT res = _channel->getDSPClock(NULL, &parentclock);
		res = _channel->addFadePoint(parentclock, 0.0f);
		res = _channel->addFadePoint(parentclock + 500000, 1.0f);
	}
	void fadeOut() {
		unsigned long long parentclock;
		FMOD_RESULT res = _channel->getDSPClock(NULL, &parentclock);
		float vol; 
		_channel->getVolume(&vol);
		res = _channel->addFadePoint(parentclock, vol);
		res = _channel->addFadePoint(parentclock + 500000, 0.0f);
	}
	
private:

	Sound * _sound;
	Channel * _channel;
	
};




int main() {
	if (syst == NULL) {
		//System init
		FMOD_RESULT res;
		res = System_Create(&syst);
		ERRCHECK(res);

		res = syst->init(128, FMOD_INIT_NORMAL, 0);
		ERRCHECK(res);
	}
	PotatoSOUND a("../muestras/talking.wav");
	a.PlaySound();
	a.fadeOut();

	while (true)
		syst->update();



	FMOD_RESULT res = syst->release();
	ERRCHECK(res);
	return 0;
}