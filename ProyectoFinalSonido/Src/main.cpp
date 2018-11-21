#include <fmod.h>
#include <fmod.hpp>
#include <SDL2/SDL.h>
#include "Looper.h"

int main(int argc, char *args[])
{


	SDL_Window *window = NULL;
	return 0;

	Looper * a = new Looper();
	if (!a->init())exit(-1);
	while (a->run());
	a->release();
	delete a;
	exit(0);
}