#pragma once
class SDL_Renderer;
class HUDRenderable
{
public:
	virtual void render(SDL_Renderer * r) = 0;
protected:
	HUDRenderable() {};
};
