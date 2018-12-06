#include "Looper.h"
#include <exception>
#include <iostream>
#include "Utilities.h"
#include <SDL2\SDL.h>
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
	_channels[_activeChannel]->loadSound("cmon.wav");
	_activeChannel++;
	_channels[_activeChannel]->loadSound("t1.wav");
	_activeChannel++;
	_channels[_activeChannel]->loadSound("t2.wav");
	_activeChannel++;
	_channels[_activeChannel]->loadSound("t3.wav");
	_activeChannel++;
	_channels[_activeChannel]->loadSound("t4.wav");

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

void Looper::playChannel(const int & n)
{
	if (n < 0 || n> NUM_LOOPER_CHANNELS)
		throw new std::exception("CHANNEL NUMBER OUT OF CHANNEL INDEX");

	_channels[n]->playSound();
}

bool Looper::run() {
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
	SDL_Event e;
	SDL_PollEvent(&e);
	if (e.type == SDL_KEYDOWN) {
		switch (e.key.keysym.sym) {
		case (SDLK_1):
			_activeChannel = 0;
			playChannel(_activeChannel);
			break;
		case (SDLK_2):
			_activeChannel = 1;
			playChannel(_activeChannel);
			break;
		case (SDLK_3):
			_activeChannel = 2;
			playChannel(_activeChannel);
			break;
		case (SDLK_4):
			_activeChannel = 3;
			playChannel(_activeChannel);
			break;
		case (SDLK_5):
			_activeChannel = 4;
			playChannel(_activeChannel);
			break;
		case (SDLK_6):
			_activeChannel = 5;
			playChannel(_activeChannel);
			break;

		case(SDLK_l):
			LooperChannel * aux = _channels[_activeChannel];
			aux->setLooping(!aux->getLooping());
			std::cout << "Looping is now: "<< aux->getLooping() << std::endl;
		}
			
	}
	


	return true;
}

