#pragma once
#include <fmod.h>
#include <fmod.hpp>
#include <fmod_errors.h>
#include "LooperChannel.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector> 

//#define NUM_LOOPER_CHANNELS 9

enum EditMode { NOTHING, PLAY, STOP, LOOP, VOLUME, PITCH, FLANGER};

enum addMode {NOADD, ADD, REMOVE};

class Looper {

public:
	static const int WIN_HEIGHT = 680;
	static const int WIN_WIDTH = 900;	
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

	void loadFont(std::string text);
	SDL_Texture* loadTexture(std::string path);
	void render();	

	bool loadFromRenderedText(std::string textureText, SDL_Color textColor);

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
	SDL_Texture* selector;

	//Selector
	SDL_Rect selecPos;

	//Font
	TTF_Font *gFont = NULL; //Arial
	//std::vector<SDL_Texture*> textos;
	SDL_Texture* textFont;
	SDL_Rect textPos;	

};

