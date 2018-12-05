#include "Recorder.h"
#include <iostream>


static FMOD::System * _system;
static FMOD::Sound * recordedS;
static FMOD_CREATESOUNDEXINFO exinfo;
static int nativeRate;
static int nativeChannels;

static unsigned int driftThreshold;
static unsigned int desiredLatency;
static unsigned int adjustedLatency;
static int actualLatency;

void Recorder::init(FMOD::System * syst)
{
	
	/*
		Determine latency in samples.
	*/
	_system = syst;
	nativeRate = 0;
	nativeChannels = 0;
	FMOD_RESULT result;
	result = _system->getRecordDriverInfo(DEVICE_INDEX, NULL, 0, NULL, &nativeRate, NULL, &nativeChannels, NULL);
	ERRCHECK(result);

	driftThreshold = (nativeRate * DRIFT_MS) / 1000;       /* The point where we start compensating for drift */
	desiredLatency = (nativeRate * LATENCY_MS) / 1000;     /* User specified latency */
	adjustedLatency = desiredLatency;                      /* User specified latency adjusted for driver update granularity */
	actualLatency = desiredLatency;               /* Latency measured once playback begins (smoothened for jitter) */


	exinfo = { 0 };
	exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
	exinfo.numchannels = nativeChannels;
	exinfo.format = FMOD_SOUND_FORMAT_PCM16;
	exinfo.defaultfrequency = nativeRate;
	exinfo.length = nativeRate* sizeof(short) * nativeChannels; /* 1 second buffer, size here doesn't change latency */
	recordedS = NULL;
	result =_system->createSound(0, FMOD_LOOP_NORMAL | FMOD_OPENUSER, &exinfo, &recordedS);

	std::cout << "Recorder Inicializado" << std::endl;
}

int Recorder::getNativeRate()
{
	return nativeRate;
}

int Recorder::getNativeChannels()
{
	return nativeChannels;
}

int Recorder::startRecording()
{
	return _system->recordStart(DEVICE_INDEX, recordedS, true);
}

FMOD::Sound * Recorder::stopRecording()
{
	_system->recordStop(DEVICE_INDEX);
	FMOD::Sound * aux = recordedS;
	recordedS = nullptr;
	return aux;
}

bool Recorder::isRecording()
{
	bool isRecording;
	_system->isRecording(DEVICE_INDEX, &isRecording);
	return isRecording;
}
