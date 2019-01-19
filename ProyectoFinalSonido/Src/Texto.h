#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <stdio.h>
#include <string>
#include <iostream>

class Texto{
public:
	Texto(std::string cadena, SDL_Renderer* renderer);
	~Texto();

	SDL_Texture* getTexture(){ return textFont; }
	TTF_Font * getFont() { return gFont; }
	SDL_Rect getRect(){ return textPos; }
	void setString(std::string text);
	void setPosition(int x, int y);
	void setRectX(int x){ textPos.x = x; }

private:
	bool load(std::string textureText, SDL_Color textColor);
	void loadFont(std::string text);

	SDL_Texture* textFont;
	SDL_Rect textPos;
	TTF_Font *gFont = NULL;
	int x_, y_;
	SDL_Renderer* renderer_;

	SDL_Color textColor;
	SDL_Surface* textSurface;
};