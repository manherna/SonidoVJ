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
		res = _system->createSound("../Sounds/hiphop.wav", FMOD_LOOP_NORMAL, 0, &_sound);
		ERRCHECK(res);
		res = _system->playSound(_sound, 0, false, &_channel);
		ERRCHECK(res);
		res = _system->createSound("../Sounds/cmon.wav", 0, 0, &_sound);
		ERRCHECK(res);
		res = _system->playSound(_sound, 0, true, &_channel);
		ERRCHECK(res);
	}
	catch (std::exception & e){
		std::cout << "An exception has ocurred: " << e.what() << std::endl;
		system("PAUSE");
		return false;
	}

	return true;
}
void Looper::release() {
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
		if(e.key.keysym.sym == SDLK_c )
			_system->playSound(_sound, 0, false, &_channel);
	}
	


	return true;
}

