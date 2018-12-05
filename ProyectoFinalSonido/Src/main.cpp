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
	SDL_Window * z = SDL_CreateWindow("Hola holita", 0, 0, 800, 600, 0);

	Looper * a = new Looper();
	if (!a->init())exit(-1);
	while (a->run());
	a->release();
	delete a;
	exit(0);
}