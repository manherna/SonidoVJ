#pragma once
#include <fmod.h>
#include <fmod.hpp>
#include <fmod_errors.h>


enum EditMode { NOTHING = 0, MUTE = 1, VOLUME = 2, PITCH = 3, FLANGER = 4, };

class Looper {
public:
	Looper();
	~Looper();


	bool run();
	bool init();
	void release();
	void processKeys();

private:
	int _activeChannel;
	EditMode _activeMode;
	FMOD::System * _system;
	//TODO: REMOVE THIS. DEBUG PURPOSE ONLY
	FMOD::Sound *_sound;
	FMOD::Sound *_snd2;

	FMOD::Channel *_channel;
	FMOD::Channel *_channel2;
};

