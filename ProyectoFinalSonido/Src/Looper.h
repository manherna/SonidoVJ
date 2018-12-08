#pragma once
#include <fmod.h>
#include <fmod.hpp>
#include <fmod_errors.h>
#include "LooperChannel.h"

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

#define NUM_LOOPER_CHANNELS 9

enum EditMode { NOTHING, PLAY, STOP, LOOP, VOLUME, PITCH, FLANGER};

class Looper {	
public:
	static const int WIN_HEIGHT = 600;
	static const int WIN_WIDTH = 800;	
	static SDL_Renderer* renderer;
	static SDL_Window * window;

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

	SDL_Texture* loadMedia(std::string path);
	void render();

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

	//Images
	SDL_Texture* emptySound;
	SDL_Texture* fullSound;	
};

