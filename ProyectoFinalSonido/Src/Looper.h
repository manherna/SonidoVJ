#pragma once
#include <fmod.h>
#include <fmod.hpp>
#include <fmod_errors.h>
#include "LooperChannel.h"

#define NUM_LOOPER_CHANNELS 8

enum EditMode { NOTHING = 0, MUTE = 1, VOLUME = 2, PITCH = 3, FLANGER = 4, };

class Looper {
public:
	Looper();
	~Looper();

	bool run();
	bool init();
	void release();
	void processKeys();
	void playChannel(const int &n);
	void pauseChannel(const int & nc);

private:
	FMOD::System * _system;
	LooperChannel * _channels[NUM_LOOPER_CHANNELS];

	EditMode _activeMode;
	short _activeChannel;


	//TODO: REMOVE THIS. DEBUG PURPOSE ONLY
	FMOD::Sound *_sound;
	FMOD::Sound *_snd2;

	FMOD::Channel *_channel;
	FMOD::Channel *_channel2;
};

