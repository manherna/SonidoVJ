#pragma once
#include <fmod.h>
#include <fmod.hpp>
#include <fmod_errors.h>
#include "LooperChannel.h"
#include <SDL2\SDL.h>
#define NUM_LOOPER_CHANNELS 9

enum EditMode { NOTHING, PLAY, STOP, LOOP, VOLUME, PITCH, FLANGER};

class Looper {
public:
	Looper();
	~Looper();

	bool run();
	bool init();
	void release();	

private:
	void handleEvents();
	void processKeys(const SDL_Event & e);
	void processDrop(const SDL_Event & e);
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
	short _activeChannel;
	bool _keypressed;

	//TODO: REMOVE THIS. DEBUG PURPOSE ONLY
	FMOD::Sound *_sound;
	FMOD::Sound *_snd2;

	FMOD::Channel *_channel;
	FMOD::Channel *_channel2;
};

