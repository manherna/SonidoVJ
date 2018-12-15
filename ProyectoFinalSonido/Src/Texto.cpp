#include "Texto.h"

Texto::Texto(std::string cadena, SDL_Renderer* renderer)
{
	loadFont(cadena, renderer);
	//renderer_ = renderer;
}
Texto::~Texto()
{
	SDL_free(textFont);		
	SDL_free(gFont);	
}

void Texto::setString(std::string text, SDL_Renderer* renderer)
{
	loadFont(text, renderer);
	setPosition(x_, y_);
}

void Texto::setPosition(int x, int y)
{
	x_ = x;
	y_ = y;
	textPos = { x_, y_, textPos.w, textPos.h };
}

void Texto::loadFont(std::string text, SDL_Renderer* renderer)
{

#if Debug
	//Open the font
	gFont = TTF_OpenFont("../Fonts/arial.ttf", 14);
#else
	gFont = TTF_OpenFont("../../Fonts/arial.ttf", 14);
#endif

	if (gFont == NULL)
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
	else
	{
		//Render text
		SDL_Color textColor = { 255, 255, 255 };
		if (!load(text.c_str(), textColor, renderer))
			printf("Failed to render text texture!\n");
	}
}

bool Texto::load(std::string textureText, SDL_Color textColor, SDL_Renderer* renderer)
{
	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		textFont = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (textFont == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions			
			textPos = { 25, 620, textSurface->w, textSurface->h };			
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	//Return success
	return textFont != NULL;
}