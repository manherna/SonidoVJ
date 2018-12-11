#include <fmod.h>
#include <fmod.hpp>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "Looper.h"

int main(int argc, char *args[])
{
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) ==-1)
		return  -1;

	Looper * looper = new Looper();

	if (!looper->init())exit(-1);
	while (looper->run());
	looper->release();
	delete looper;
	exit(0);
}