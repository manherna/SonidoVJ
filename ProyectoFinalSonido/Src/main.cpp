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

	//Create window
	Looper::window = SDL_CreateWindow("Looper", 300, 300, Looper::WIN_WIDTH, Looper::WIN_HEIGHT, SDL_WINDOW_SHOWN);
	if (Looper::window == NULL)
	{
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());		
	}
	else
	{
		//Create renderer for window
		Looper::renderer = SDL_CreateRenderer(Looper::window, -1, SDL_RENDERER_ACCELERATED);
		if (Looper::renderer == NULL)
		{
			printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());			
		}
		else
		{
			//Initialize renderer color
			SDL_SetRenderDrawColor(Looper::renderer, 0x00, 0x30, 0x66, 0xFF);

			//Initialize PNG loading
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) & imgFlags))
			{
				printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());				
			}
		}
	}
	
	if (!looper->init())exit(-1);
	while (looper->run());
	looper->release();
	delete looper;
	exit(0);
}