#include <fmod.hpp>
#include <fmod_errors.h>
#include <iostream>
#include <thread>
#include <cstdio>
#include <conio.h>

using namespace FMOD;
System * syst;

void ERRCHECK(FMOD_RESULT result)
{
	if (result != FMOD_OK) {
		std::cout << FMOD_ErrorString(result) << std::endl;
		//exit(-1);
	}

}

class PotatoSOUND
{	
public:
	PotatoSOUND(){}
	PotatoSOUND(const char * path) 
	{
		FMOD_RESULT res;
		res = syst->createSound(path, FMOD_DEFAULT, 0, &_sound);
		ERRCHECK(res);
		res = syst->playSound(_sound, 0, true, &_channel);
		ERRCHECK(res);
	}
	~PotatoSOUND()
	{
		_sound->release();
	}
	void PlaySound()
	{
		FMOD_RESULT res = syst->playSound(_sound, 0, false, &_channel);
		ERRCHECK(res);

	}
	void togglePause()
	{	
		bool paused;
		_channel->getPaused(&paused);
		_channel->setPaused(!paused);	

	}
	void setPitch(float i)
	{
		FMOD_RESULT res;
		res = _channel->setPitch(i);
		ERRCHECK(res);
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

template <class T>
int getIndex(T * arr, T find, int size) {
	int z = -1;
	for (int i = 0; i < size; i++) {
		if (arr[i] == find)return i;
	}
	return z;
}


int main() {
	if (syst == NULL) {
		//System init
		FMOD_RESULT res;
		res = System_Create(&syst);
		ERRCHECK(res);

		res = syst->init(128, FMOD_INIT_NORMAL, 0);
		ERRCHECK(res);
	}
	char * notas = "cdefgab";
	char * teclas = "zxcvbnm,";
	short int frec[8] = {0,2,4,5,7,9,11,12};
	float pitch [8];
	PotatoSOUND * sounds [8];

	for (int i = 0; i < 8; i++) {
		pitch[i] = std::pow(2, (frec[i] / 12.0f));
		std::cout << pitch[i] << std::endl;
		sounds[i] = new PotatoSOUND("../muestras/piano.ogg");
	}

	int i = 0;
	while (true) {
		if(_kbhit()){
			char a = _getch();
			if (a == 'q')break;
			else {
				int i = getIndex(teclas, a, 8);
				if (i == -1)continue;
				else {
					std::cout << notas[i] << " pitch: " << pitch[i] << std::endl;
					sounds[i]->PlaySound();
					sounds[i]->setPitch(pitch[i]);
				}
			}
		}
		syst->update();	
	}
	std::cout << "Closing.." << std::endl;
	for (int i = 0; i < 8; i++) {
		delete sounds[i];
	}
	FMOD_RESULT res = syst->release();
	ERRCHECK(res);
	return 0;
}