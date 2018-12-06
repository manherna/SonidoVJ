#include <fmod.h>
#include <fmod.hpp>
#include <SDL2/SDL.h>
#include "Looper.h"
#include <SDL2\SDL.h>

int main(int argc, char *args[])
{


	SDL_Window *window = NULL;

	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) ==-1)
		return  -1;

	SDL_Window * z = SDL_CreateWindow("Looper", 300, 300, 300, 300, 0);

	Looper * looper = new Looper();
	if (!looper->init())exit(-1);
	while (looper->run());
	looper->release();
	delete looper;
	exit(0);
}