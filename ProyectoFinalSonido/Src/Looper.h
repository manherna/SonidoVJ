#pragma once
#include <fmod.h>
#include <fmod.hpp>
#include <fmod_errors.h>
#include "LooperChannel.h"

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector> 

//#define NUM_LOOPER_CHANNELS 9

enum EditMode { NOTHING, PLAY, STOP, LOOP, VOLUME, PITCH, FLANGER};

enum addMode {NOADD, ADD, REMOVE};

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
	void deleteSound(int n);

	SDL_Texture* loadTexture(std::string path);
	void render();	

	char* dropped_filedir;	

	FMOD::System * _system;
	
	//LooperChannel * _channels[numChannels];
	std::vector<LooperChannel*> _channels;
	int numChannels = 0;

	EditMode _activeMode;
	EditMode _lastActiveMode;
	addMode _lastAddMode;
	addMode _addMode;
	short _activeChannel;
	bool _keypressed;

	//Images
	SDL_Texture* emptySound;
	SDL_Texture* fullSound;

};

