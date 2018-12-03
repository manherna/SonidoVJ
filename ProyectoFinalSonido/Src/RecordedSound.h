#include "fmod.hpp"
#include "common.h"
#include "FMODSystem.h"

class RecordedSound {

public:
	RecordedSound(FMODSystem *s);
	~RecordedSound() {}

	FMOD::Channel* getChannel() { return channel; }
	void startRecording();
	void stopRecording();
	void playRecord();

	bool isRecording();
	
private:

	FMOD::Channel *channel = NULL;
	FMOD_CREATESOUNDEXINFO exinfo;
	FMOD::Sound *sound;
	FMODSystem *system_;
	int result;

	//Vector de efectos? (o solo uno?)
	/*void *extraDriverData = NULL;
    Common_Init(&extraDriverData);*/	
};