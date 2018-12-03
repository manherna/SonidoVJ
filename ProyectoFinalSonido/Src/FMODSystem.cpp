#include "FMODSystem.h"
#include <iostream>

FMODSystem::FMODSystem() 
{
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

	/*
		Determine latency in samples.
	*/
	nativeRate = 0;
	nativeChannels = 0;
	result = system->getRecordDriverInfo(DEVICE_INDEX, NULL, 0, NULL, &nativeRate, NULL, &nativeChannels, NULL);
	ERRCHECK(result);

	driftThreshold = (nativeRate * DRIFT_MS) / 1000;       /* The point where we start compensating for drift */
	desiredLatency = (nativeRate * LATENCY_MS) / 1000;     /* User specified latency */
	adjustedLatency = desiredLatency;                      /* User specified latency adjusted for driver update granularity */
	actualLatency = desiredLatency;                                 /* Latency measured once playback begins (smoothened for jitter) */

	std::cout << "Sistema FMOD inicializado con exito" << std::endl;
}