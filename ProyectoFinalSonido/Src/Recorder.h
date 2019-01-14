#pragma once
#include "common.h"
#include "fmod.hpp"

#define LATENCY_MS      (50) /* Some devices will require higher latency to avoid glitches */
#define DRIFT_MS        (1)
#define DEVICE_INDEX    (0)

class Recorder {
public:
	static void init(FMOD::System * syst);
	static void doubleLength();

	static void setLength(int n);	
	static void release();
	static bool isRecording();
	static int getNativeRate();
	static int getNativeChannels();
	static int startRecording();
	static FMOD::Sound * stopRecording();

	static unsigned int adjustedLatency;
	static unsigned int desiredLatency;
	static int nativeRate;
	static unsigned int driftThreshold;
	static int actualLatency;
	static FMOD_CREATESOUNDEXINFO exinfo;
};
