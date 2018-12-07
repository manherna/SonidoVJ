#include "Looper.h"
#include <exception>
#include <iostream>
#include "Utilities.h"
#include <SDL2\SDL.h>
#include <stdlib.h>
Looper::Looper() {

}
Looper::~Looper() {

}

bool Looper::init() 
{
	FMOD_RESULT res;
	try {
		res = System_Create(&_system);
		ERRCHECK(res);
		res = _system->init(128, FMOD_INIT_NORMAL, 0);
		ERRCHECK(res);


		//TODO: REMOVE THIS. DEBUG PURPOSE ONLY
	}
	catch (std::exception & e){
		std::cout << "An exception has ocurred: " << e.what() << std::endl;
		system("PAUSE");
		return false;
	}
	for (int i = 0; i < NUM_LOOPER_CHANNELS; i++)
	{
		_channels[i] = new LooperChannel(_system, i);
	}

	_activeChannel = 0;
	_channels[_activeChannel]->loadSound("hiphop.wav");
	_activeChannel++;	
	_channels[_activeChannel]->loadSound("one.wav");
	_activeChannel++;
	_channels[_activeChannel]->loadSound("t1.wav");
	_activeChannel++;
	_channels[_activeChannel]->loadSound("t2.wav");
	_activeChannel++;
	_channels[_activeChannel]->loadSound("t3.wav");
	_activeChannel++;
	_channels[_activeChannel]->loadSound("t4.wav");
	_activeChannel++;
	_channels[_activeChannel]->loadSound("uh.wav");
	_activeChannel++;
	_channels[_activeChannel]->loadSound("again.wav");
	_activeChannel = -1;
	return true;


}
void Looper::release() {

	for (int i = 0; i < NUM_LOOPER_CHANNELS; i++)delete _channels[i];
	FMOD_RESULT res;
	res = _system->release();
	try
	{
		ERRCHECK(res);
	}
	catch (const std::exception& e)
	{
		std::cout << "Exception on release: " << e.what() << std::endl;
		system("PAUSE");
		return;
	}
}

void Looper::handleEvents()
{
	SDL_Event e;
	SDL_PollEvent(&e);
	switch(e.type)
	{
	case(SDL_KEYDOWN):
		processKeys(e);
		break;
	case(SDL_DROPFILE):
		processDrop(e);
		break;
	}

}

void Looper::processKeys(const SDL_Event & e)
{

	_keypressed = true;
	switch (e.key.keysym.sym) {
	case (SDLK_1):
		_activeChannel = 0;
		break;
	case (SDLK_2):
		_activeChannel = 1;
		break;
	case (SDLK_3):
		_activeChannel = 2;
		break;
	case (SDLK_4):
		_activeChannel = 3;
		break;
	case (SDLK_5):
		_activeChannel = 4;
		break;
	case (SDLK_6):
		_activeChannel = 5;
		break;
	case (SDLK_7):
		_activeChannel = 6;
		break;
	case (SDLK_8):
		_activeChannel = 7;
		break;
	case (SDLK_9):
		_activeChannel = 8;
		break;
	case (SDLK_0):
		_activeChannel = -1;
		break;
	case (SDLK_p):
		_lastActiveMode = _activeMode;
		_activeMode = PLAY;
		break;
	case (SDLK_s):
		_lastActiveMode = _activeMode;
		_activeMode = STOP;
		break;
	case (SDLK_o):
		_lastActiveMode = _activeMode;
		_activeMode = PITCH;
		break;
	case (SDLK_v):
		_lastActiveMode = _activeMode;
		_activeMode = VOLUME;
		break;
	case (SDLK_l):
		_lastActiveMode = _activeMode;
		_activeMode = LOOP;
		break;
	case (SDLK_n):
		_lastActiveMode = _activeMode;
		_activeMode = NOTHING;
	case(SDLK_SPACE):
		_lastActiveMode = _activeMode;
		_activeMode = NOTHING;
		_activeChannel = -1;
	default:
		_keypressed = false;
		break;
	}
}
void Looper::processDrop(const SDL_Event & e)
{
	dropped_filedir = e.drop.file;
	archivoCaido = true;
	_channels[_activeChannel]->loadFile(dropped_filedir);

}


void Looper::processState()
{
	if (!_keypressed || (_activeChannel < 0)) return;
	switch (_activeMode)
	{
	case(PLAY):
		playChannel(_activeChannel);
		break;
	
	//TODO:CASE(STOP)
	case(LOOP):
		toggleLoopChannel(_activeChannel);
		break;
	}
	
	_keypressed = false;

}

void Looper::playChannel(const int & n)
{
	if (n < 0 || n> NUM_LOOPER_CHANNELS)
		throw new std::exception("CHANNEL NUMBER OUT OF CHANNEL INDEX");

	_channels[n]->playSound();
}

void Looper::toggleLoopChannel(const int & n)
{
	_channels[n]->setLooping(!_channels[n]->getLooping());
}

void Looper::printHUD()
{
	printf("Active Mode: %d\n", _activeMode);
	printf("Active Channel %d\n", _activeChannel);
	printf("-------------------------------------------------------------------\n");
	printf("Channels can be selected with numbers 1-8\n");
	printf("LOOPER MODES:\n");
	printf("P: PLAY MODE. Select a Channel and then P to play it\n");
	printf("L: LOOP MODE. Select a Channel and then L to toggle its loop state\n");
	printf("Channels can be selected with numbers 1-8\n");
	printf("-------------------------------------------------------------------\n");

	if(archivoCaido)
		printf("Loaded file: %s\n", dropped_filedir);

}


bool Looper::run() {
	system("CLS");

	handleEvents();
	processState();	


	FMOD_RESULT res = _system->update();
	try
	{
		ERRCHECK(res);
	}
	catch (const std::exception& e)
	{
		std::cout << "An exception has ocurred in run(): " << e.what() << std::endl;
		system("PAUSE");
		return false;
	}
	printHUD();
	return true;
}

