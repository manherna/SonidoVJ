#pragma once
#include "ConcurrentQueue.h"
#include <SDL2\SDL.h>
class ClickListener;
class LooperHUD
{
public:
	static bool init(const char * appName);
	static bool release();
	static void addClickListener(ClickListener * c);
	static void notifyClickListeners(SDL_Event & e);
	static void renderHUD();
	static void HUDUpdate();

};

