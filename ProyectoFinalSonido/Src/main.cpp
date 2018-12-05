#include <fmod.h>
#include <fmod.hpp>
#include <SDL2/SDL.h>
#include "Looper.h"
#include "Recorder.h"
#include <iostream>
#include <conio.h>

//int main(int argc, char *args[]) //SDL, se puede cambiar el punto de entrada para que funcionen ambos

int FMOD_Main()
{
	//SDL_Window *window = NULL;
	//return 0;
	FMOD::System * system = nullptr;
	void *extraDriverData = NULL;
	Common_Init(&extraDriverData); //Init FMOD

	FMOD_RESULT result = FMOD::System_Create(&system);
	ERRCHECK(result);

	unsigned int version = 0;
	result = system->getVersion(&version);
	ERRCHECK(result);

	if (version < FMOD_VERSION)
	{
		Common_Fatal("FMOD lib version %08x doesn't match header version %08x", version, FMOD_VERSION);
	}

	result = system->init(100, FMOD_INIT_NORMAL, extraDriverData);
	ERRCHECK(result);

	int numDrivers = 0;
	result = system->getRecordNumDrivers(NULL, &numDrivers);
	ERRCHECK(result);

	if (numDrivers == 0)
	{
		Common_Fatal("No recording devices found/plugged in!  Aborting.");
	}

	std::cout << "Comenzando programa..." << std::endl;

	FMOD::Sound * snd = nullptr;
	FMOD::Channel * chnel = nullptr;

	Recorder::init(system);

	while (true) 
	{
		if (_kbhit()) {
			char c = getchar();

			if (c == 'r')
			{
				if (!Recorder::isRecording()) {
					if (snd != nullptr)delete snd;
					Recorder::startRecording();
				}
				else {
					snd = Recorder::stopRecording();
					system->playSound(snd, NULL, false, &chnel);
					
				}
			}

		}
		system->update();
	}
	
	exit(0);
}