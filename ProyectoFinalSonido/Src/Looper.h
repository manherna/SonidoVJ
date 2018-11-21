#pragma once
#include <fmod.h>
#include <fmod.hpp>
enum editMode {
	NULL,
	MUTE,
	VOLUME,
	PITCH,
	FLANGER
};

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
	editMode _activeMode;
};