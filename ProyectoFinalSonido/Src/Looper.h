#pragma once
#include <fmod.h>
#include <fmod.hpp>
#include <fmod_errors.h>
#include "LooperChannel.h"

#define NUM_LOOPER_CHANNELS 9

enum EditMode { NOTHING, PLAY, STOP, LOOP, VOLUME, PITCH, FLANGER};
enum addMode {NOADD, ADD, REMOVE};
class Looper {
public:
	Looper();
	~Looper();

	bool run();
	bool init();
	void release();	

private:
	void processKeys();
	void processDrop();
	void loadSound();
	void processState();
	void playChannel(const int &n);
	void pauseChannel(const int & nc);
	void toggleLoopChannel(const int & n);
	void printHUD();

	char* dropped_filedir;
	bool archivoCaido = false;

	FMOD::System * _system;
	LooperChannel * _channels[NUM_LOOPER_CHANNELS];

	EditMode _activeMode;
	EditMode _lastActiveMode;
	addMode _lastAddMode;
	addMode _addMode;
	short _activeChannel;
	bool _keypressed;
};

