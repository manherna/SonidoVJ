#pragma once
#include <SDL2\SDL.h>
#include "ClickListener.h"
#include "HUDRenderable.h"

class HUDButton: public ClickListener, public HUDRenderable
{
public:
	HUDButton();
	~HUDButton();
	virtual void OnClick(SDL_Event &e);
	virtual void OnClickUp(SDL_Event &e);
	virtual void render(SDL_Renderer * r);
	SDL_Rect getPosition();
	void setPosition(const SDL_Rect & nPos);


private:
	bool isInsideRect(const int & x, const int & y);
	bool clicked;
	SDL_Rect _src;
	SDL_Rect _dest;

};