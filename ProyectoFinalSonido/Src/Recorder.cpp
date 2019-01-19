#include "Recorder.h"
#include <iostream>
#include <time.h>

static FMOD::System * _system;
static FMOD::Sound * recordedS;
FMOD_CREATESOUNDEXINFO Recorder::exinfo;
int Recorder::nativeRate;
static int nativeChannels;
static clock_t ini;
static clock_t fin;

unsigned int Recorder::driftThreshold;
unsigned int Recorder::desiredLatency;
unsigned int Recorder::adjustedLatency;
int Recorder::actualLatency;

void Recorder::init(FMOD::System * syst)
{		
	_system = syst;
	nativeRate = 0;
	nativeChannels = 0;
	FMOD_RESULT result;
	result = _system->getRecordDriverInfo(DEVICE_INDEX, NULL, 0, NULL, &nativeRate, NULL, &nativeChannels, NULL);	

	driftThreshold = (nativeRate * DRIFT_MS) / 1000;       /* The point where we start compensating for drift */
	desiredLatency = (nativeRate * LATENCY_MS) / 1000;     /* User specified latency */
	adjustedLatency = desiredLatency;                      /* User specified latency adjusted for driver update granularity */
	actualLatency = desiredLatency;               /* Latency measured once playback begins (smoothened for jitter) */
	
	exinfo = { 0 };
	exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
	exinfo.numchannels = nativeChannels;
	exinfo.format = FMOD_SOUND_FORMAT_PCM16;
	exinfo.defaultfrequency = nativeRate;
	exinfo.length = 90000000; //nativeRate * sizeof(short) * nativeChannels; //* 15; /* 1 second buffer, size here doesn't change latency */
	
	std::cout << "Recorder Inicializado" << std::endl;
}

void Recorder::setLength(int n)
{
	exinfo.length = n;
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
	recordedS = NULL;
	FMOD_RESULT result = _system->createSound(0, FMOD_LOOP_NORMAL | FMOD_OPENUSER, &exinfo, &recordedS);
	result = _system->recordStart(DEVICE_INDEX, recordedS, true);
	ini = clock();
	return  result;
}

FMOD::Sound * Recorder::stopRecording()
{
	fin = clock();
	int a = _system->recordStop(DEVICE_INDEX);
	std::cout << (fin - ini) * 1000 << " " << a << std::endl;
	recordedS->setLoopPoints(0, FMOD_TIMEUNIT_MS, (fin - ini) * 1000 / (CLOCKS_PER_SEC), FMOD_TIMEUNIT_MS);

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
