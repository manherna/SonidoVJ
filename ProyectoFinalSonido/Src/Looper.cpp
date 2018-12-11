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
	}
	catch (std::exception & e){
		std::cout << "An exception has ocurred: " << e.what() << std::endl;
		system("STOP");
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
	_activeChannel = 0;
	
	
	_activeMode = NOTHING;
	_lastActiveMode = NOTHING;
	_addMode = NOADD;
	_lastAddMode = NOADD;
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
		system("STOP");
		return;
	}
}

void Looper::processKeys()
{
	SDL_Event e;
	SDL_PollEvent(&e);
	if (e.type == SDL_KEYDOWN) {
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
		case(SDLK_PLUS):
			_lastAddMode = _addMode;
			_addMode = ADD;
			break;
		case(SDLK_MINUS):
			_lastAddMode = _addMode;
			_addMode = REMOVE;
			break;

		//This cases are to clean the channel and
		//State selection
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
}

void Looper::processState()
{
	if (!_keypressed || (_activeChannel < 0)) return;
	switch (_activeMode)
	{
	case(PLAY):
		playChannel(_activeChannel);
		break;
	case (STOP):
		pauseChannel(_activeChannel);
		break;
	case(LOOP):
		toggleLoopChannel(_activeChannel);
		break;
	case(VOLUME):
		if (_lastAddMode != _addMode)
			if (_addMode == ADD) 
				_channels[_activeChannel]->setVolume(_channels[_activeChannel]->getVolume() + 0.1f);
			else if(_addMode = REMOVE)
				_channels[_activeChannel]->setVolume(_channels[_activeChannel]->getVolume() - 0.1f);
		_addMode = NOADD;
	case(PITCH):
		if (_lastAddMode != _addMode)
			if (_addMode == ADD)
				_channels[_activeChannel]->setPitch(_channels[_activeChannel]->getPitch() + 0.1f);
			else if (_addMode = REMOVE)
				_channels[_activeChannel]->setPitch(_channels[_activeChannel]->getPitch() - 0.1f);
		_addMode = NOADD;
	}

	
	
	_keypressed = false;

}

void Looper::playChannel(const int & n)
{
	if (n < 0 || n> NUM_LOOPER_CHANNELS)
		throw new std::exception("CHANNEL NUMBER OUT OF CHANNEL INDEX");

	_channels[n]->playSound();
}

void Looper::pauseChannel(const int & nc)
{
	_channels[nc]->pauseSound();
}

void Looper::toggleLoopChannel(const int & n)
{
	_channels[n]->setLooping(!_channels[n]->getLooping());
}

void Looper::printHUD()
{
	printf("Active Mode: %d\n", _activeMode);
	printf("Active Channel %d\n", _activeChannel);
	if (_activeChannel >= 0 && _activeChannel < NUM_LOOPER_CHANNELS) {
		printf("Active Channel volume %f\n", _channels[_activeChannel]->getVolume());
		printf("Active Channel pitch %f\n", _channels[_activeChannel]->getPitch());
	}
		printf("-------------------------------------------------------------------\n");
	printf("Channels can be selected with numbers 1-8\n");
	printf("LOOPER MODES:\n");
	printf("P: PLAY MODE. Select a Channel and then P to play it\n");
	printf("L: LOOP MODE. Select a Channel and then L to toggle its loop state\n");
	printf("S: STOP MODE. Select a Channel and then P to stop a sound from playing\n");
	
	printf("Channels can be selected with numbers 1-8\n");
	printf("-------------------------------------------------------------------\n");

	if(archivoCaido)
		printf("Loaded file: %s\n", dropped_filedir);

}

void Looper::processDrop()
{
	SDL_Event e;
	SDL_PollEvent(&e);
	if (e.type == SDL_DROPFILE) {
		dropped_filedir = e.drop.file;		

		archivoCaido = true;
		_activeChannel = 8;		
		_channels[_activeChannel]->loadFile(dropped_filedir);		
	}
}

bool Looper::run() {
	system("CLS");

	processKeys();
	processDrop();
	processState();	


	FMOD_RESULT res = _system->update();
	try
	{
		ERRCHECK(res);
	}
	catch (const std::exception& e)
	{
		std::cout << "An exception has ocurred in run(): " << e.what() << std::endl;
		system("STOP");
		return false;
	}
	printHUD();
	return true;
}

