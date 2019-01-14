#include <fmod.h>
#include <fmod.hpp>
#include "Looper.h"
#include "Recorder.h"
#include <iostream>
#include <conio.h>

//int main(int argc, char *args[]) //SDL, se puede cambiar el punto de entrada para que funcionen ambos

int FMOD_Main() {

	FMOD::Channel *channel = NULL;
	unsigned int samplesRecorded = 0;
	unsigned int samplesPlayed = 0;
	bool dspEnabled = false;

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

	Recorder::init(system);

	FMOD::Sound *sound = NULL;
	result = system->createSound(0, FMOD_LOOP_NORMAL | FMOD_OPENUSER, &Recorder::exinfo, &sound);
	ERRCHECK(result);

	//result = system->recordStart(DEVICE_INDEX, sound, true);
	//ERRCHECK(result);

	unsigned int soundLength = 0;
	result = sound->getLength(&soundLength, FMOD_TIMEUNIT_PCM);
	ERRCHECK(result);

	while (true)
	{
		//Common_Update();

		if (_kbhit()) {
			char c = _getch();

			if (c == 'r')
			{
				if (!Recorder::isRecording()) {
					//if (snd != nullptr)delete snd;
					Recorder::startRecording();
					std::cout << "Recording" << std::endl;
				}
				else {

					//Determine how much has been recorded since we last checked
					unsigned int recordPos = 0;
					result = system->getRecordPosition(DEVICE_INDEX, &recordPos);
					if (result != FMOD_ERR_RECORD_DISCONNECTED)
					{
						ERRCHECK(result);
					}
					//Recorder::setLength(recordPos);

					sound = Recorder::stopRecording();
					system->playSound(sound, NULL, false, &channel);
					std::cout << "Stopped Recording" << std::endl;
				}
			}

		}

		system->update();
		
	} //Fin del while

	exit(0);
}