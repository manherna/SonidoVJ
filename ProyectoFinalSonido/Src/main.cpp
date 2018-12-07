#include <fmod.h>
#include <fmod.hpp>
#include <SDL2/SDL.h>
#include "Looper.h"
#include <SDL2\SDL.h>
#include "LooperHUD.h"

int main(int argc, char *args[])
{

	LooperHUD::init("LOOPER");

	Looper * looper = new Looper();


	if (!looper->init())exit(-1);
	while (looper->run()) {
		LooperHUD::HUDUpdate();
	};
	looper->release();
	delete looper;

	LooperHUD::release();
	exit(0);
}