#include "RecordedSound.h"
#include <iostream>

RecordedSound::RecordedSound(FMODSystem* s)
{
	system_ = s;

	exinfo = { 0 };
	exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
	exinfo.numchannels = system_->getNativeChannels();
	exinfo.format = FMOD_SOUND_FORMAT_PCM16;
	exinfo.defaultfrequency = system_->getNativeRate();
	exinfo.length = system_->getNativeRate() * sizeof(short) * system_->getNativeChannels(); /* 1 second buffer, size here doesn't change latency */

	sound = NULL;
	result = system_->getSystem()->createSound(0, FMOD_LOOP_NORMAL | FMOD_OPENUSER, &exinfo, &sound);
	if (result == -1)
		std::cout << "Error en la creacion de sonido" << std::endl;
}

void RecordedSound::startRecording() 
{
	//result = system_->getSystem()->recordStart(DEVICE_INDEX, sound, true);
	system_->getSystem()->recordStart(DEVICE_INDEX, sound, true);	
}

void RecordedSound::stopRecording()
{
	system_->getSystem()->recordStop(DEVICE_INDEX);
}

void RecordedSound::playRecord() 
{
	system_->getSystem()->playSound(sound, 0, false, &channel);
}

bool RecordedSound::isRecording()
{
	bool isRecording;
	system_->getSystem()->isRecording(DEVICE_INDEX, &isRecording);
	return isRecording;
}
