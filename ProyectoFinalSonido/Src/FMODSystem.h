#pragma once
#include "common.h"
#include "fmod.hpp"

#define LATENCY_MS      (50) /* Some devices will require higher latency to avoid glitches */
#define DRIFT_MS        (1)
#define DEVICE_INDEX    (0)

class FMODSystem {
public:
	FMODSystem();
	FMOD::System* getSystem() { return system; }
	int getNativeRate() { return nativeRate; }
	int getNativeChannels() { return nativeChannels; }

private:
	FMOD::System *system = NULL;

	int nativeRate;
	int nativeChannels;
	
	unsigned int driftThreshold; 
	unsigned int desiredLatency; 
	unsigned int adjustedLatency;            
	int actualLatency;

};
