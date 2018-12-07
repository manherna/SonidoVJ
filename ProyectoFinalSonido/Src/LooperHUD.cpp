#include "LooperHUD.h"
#include <SDL2\SDL.h>
#include "ClickListener.h"
#include "HUDButton.h"
#include <list>
#include "HUDRenderable.h"


static SDL_Window * _window;
static SDL_Renderer * _renderer;
static std::list<ClickListener *> _clickListeners;
static std::list<HUDRenderable *> _renders;

bool LooperHUD::init(const char * appName)
{

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) == -1)
		return  false;
	_window = SDL_CreateWindow(appName, 300, 300, 400, 300, 0);
	_renderer = SDL_CreateRenderer(_window, -1, NULL);


	HUDButton * n = new HUDButton();
	addClickListener(n);
	_renders.push_back(n);
	return true;
}

bool LooperHUD::release()
{
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	SDL_Quit();

	return true;
}

void LooperHUD::addClickListener(ClickListener * c)
{
	_clickListeners.push_back(c);
}

void LooperHUD::notifyClickListeners(SDL_Event & e)
{
	if(e.type == SDL_MOUSEBUTTONDOWN)
	for (auto i : _clickListeners)i->OnClick(e);
	if(e.type == SDL_MOUSEBUTTONUP)
	for (auto i : _clickListeners)i->OnClickUp(e);

}

void LooperHUD::renderHUD()
{
	for (auto i : _renders)
		i->render(_renderer);
}

void LooperHUD::HUDUpdate()
{
	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
	SDL_RenderClear(_renderer);

	SDL_Event e;
	SDL_PollEvent( &e);
	if (&e == NULL)return;
	if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
		notifyClickListeners(e);


	renderHUD();
	SDL_RenderPresent(_renderer);
}


