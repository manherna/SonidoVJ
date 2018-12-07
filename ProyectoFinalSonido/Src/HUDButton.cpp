#include "HUDButton.h"
#include "LooperHUD.h"
#include <SDL2\SDL.h>
#include <SDL2\SDL_render.h>


HUDButton::HUDButton()
{	
	clicked = false;
	_dest = SDL_Rect();
	_dest.x = 50;
	_dest.y = 50;
	_dest.w = 50;
	_dest.h = 50;
}

HUDButton::~HUDButton()
{
}

void HUDButton::OnClick(SDL_Event &e)
{
	if (isInsideRect(e.motion.x, e.motion.y)) 
	{
		printf("click");
		clicked = true;
	}
}

void HUDButton::OnClickUp(SDL_Event & e)
{
	if (clicked)clicked = false;
}

void HUDButton::render(SDL_Renderer * r)
{
	if (!clicked)SDL_SetRenderDrawColor(r,255, 0, 0, 255);
	else SDL_SetRenderDrawColor(r, 0, 255, 0, 255);
		
	SDL_RenderFillRect(r, &_dest);
		
}

SDL_Rect HUDButton::getPosition()
{
	return _dest;
}

void HUDButton::setPosition(const SDL_Rect & nPos)
{
	_dest = nPos;
}


bool HUDButton::isInsideRect(const int & x, const int & y)
{
	return (
		(x > _dest.x && x < _dest.x + _dest.w) &&
		(y > _dest.y && y < _dest.y + _dest.y)
		);
}
