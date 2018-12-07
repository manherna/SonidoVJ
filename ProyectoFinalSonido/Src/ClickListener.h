#pragma once
class  ClickListener
{
public:
	virtual void OnClick(SDL_Event &e) = 0;
	virtual void OnClickUp(SDL_Event &e) = 0;
protected:
	ClickListener() {};

};