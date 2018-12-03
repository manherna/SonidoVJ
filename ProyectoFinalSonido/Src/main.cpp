#include <fmod.h>
#include <fmod.hpp>
#include <SDL2/SDL.h>
#include "Looper.h"
#include "FMODSystem.h"
#include <iostream>

//int main(int argc, char *args[]) //SDL, se puede cambiar el punto de entrada para que funcionen ambos

int FMOD_Main()
{
	//SDL_Window *window = NULL;
	//return 0;

	std::cout << "Comenzando programa..." << std::endl;
	FMODSystem *sistema = new FMODSystem();

	Looper * a = new Looper();
	
	if (!a->init())
		exit(-1);

	
	while (a->run()) 
	{
	
	}

	a->release();
	delete a;
	exit(0);
}