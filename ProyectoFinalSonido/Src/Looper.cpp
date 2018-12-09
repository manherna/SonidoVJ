#include "Looper.h"
#include <exception>
#include <iostream>
#include "Utilities.h"
#include <stdlib.h>

//SDL_Surface* Looper::screenSurface = NULL;
SDL_Renderer* Looper::renderer = NULL;
SDL_Window* Looper::window = NULL;

Looper::Looper() {

}

Looper::~Looper() {

}

bool Looper::init()
{
	FMOD_RESULT res;
	try {
		res = System_Create(&_system);
		ERRCHECK(res);
		res = _system->init(128, FMOD_INIT_NORMAL, 0);
		ERRCHECK(res);


		//TODO: REMOVE THIS. DEBUG PURPOSE ONLY
	}
	catch (std::exception & e){
		std::cout << "An exception has ocurred: " << e.what() << std::endl;
		system("PAUSE");
		return false;
	}
	/*
	for (int i = 0; i < _channels.size(); i++)
	{
		_channels[i] = new LooperChannel(_system, i);
	}*/

	_activeChannel = -1;

	emptySound = loadMedia("../Images/pistaVacia.png");

	if (emptySound == NULL)	
		printf("Failed to load emptySound texture\n");	
	
	fullSound = loadMedia("../Images/pistaFull.png");

	if (fullSound == NULL)
		printf("Failed to load fullSound texture\n");
		
	/*
	_channels[_activeChannel]->loadSound("../Sounds/hiphop.wav");
	_activeChannel++;
	_channels[_activeChannel]->loadSound("../Sounds/one.wav");
	_activeChannel++;
	_channels[_activeChannel]->loadSound("../Sounds/t1.wav");
	_activeChannel++;
	_channels[_activeChannel]->loadSound("../Sounds/t2.wav");
	_activeChannel++;
	_channels[_activeChannel]->loadSound("../Sounds/t3.wav");
	_activeChannel++;
	_channels[_activeChannel]->loadSound("../Sounds/t4.wav");
	_activeChannel++;
	_channels[_activeChannel]->loadSound("../Sounds/uh.wav");
	_activeChannel++;
	_channels[_activeChannel]->loadSound("../Sounds/again.wav");
	_activeChannel = -1;
	*/
	return true;


}

SDL_Texture* Looper::loadMedia(std::string path)
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

void Looper::release() {

	for (int i = 0; i < _channels.size(); i++)delete _channels[i];
	FMOD_RESULT res;
	res = _system->release();
	try
	{
		ERRCHECK(res);
	}
	catch (const std::exception& e)
	{
		std::cout << "Exception on release: " << e.what() << std::endl;
		system("PAUSE");
		return;
	}
}

void Looper::render()
{		
	SDL_RenderClear(renderer);
	SDL_Texture* aux = nullptr;

	for (int i = 0; i < _channels.size(); i++)
	{		
		if (_channels[i]->isPlaying())aux = fullSound;
		else aux = emptySound;

		SDL_RenderCopy(renderer, aux, NULL, &_channels[i]->getRect());		
	}
	
	SDL_RenderPresent(renderer);	
}

void Looper::processKeys()
{
	SDL_Event e;
	SDL_PollEvent(&e);
	if (e.type == SDL_KEYDOWN) {
		_keypressed = true;
		switch (e.key.keysym.sym) {
		case (SDLK_1) :
			_activeChannel = 0;
			break;
		case (SDLK_2) :
			_activeChannel = 1;
			break;
		case (SDLK_3) :
			_activeChannel = 2;
			break;
		case (SDLK_4) :
			_activeChannel = 3;
			break;
		case (SDLK_5) :
			_activeChannel = 4;
			break;
		case (SDLK_6) :
			_activeChannel = 5;
			break;
		case (SDLK_7) :
			_activeChannel = 6;
			break;
		case (SDLK_8) :
			_activeChannel = 7;
			break;
		case (SDLK_9) :
			_activeChannel = 8;
			break;
		case (SDLK_0) :
			_activeChannel = -1;
			break;
		case (SDLK_p) :
			_lastActiveMode = _activeMode;
			_activeMode = PLAY;
			break;
		case (SDLK_s) :
			_lastActiveMode = _activeMode;
			_activeMode = STOP;
			break;
		case (SDLK_o) :
			_lastActiveMode = _activeMode;
			_activeMode = PITCH;
			break;
		case (SDLK_v) :
			_lastActiveMode = _activeMode;
			_activeMode = VOLUME;
			break;
		case (SDLK_l) :
			_lastActiveMode = _activeMode;
			_activeMode = LOOP;
			break;
		case (SDLK_n) :
			_lastActiveMode = _activeMode;
			_activeMode = NOTHING;
		case(SDLK_SPACE) :
			_lastActiveMode = _activeMode;
			_activeMode = NOTHING;
			_activeChannel = -1;
		default:
			_keypressed = false;
			break;
		}
	}
}

void Looper::processState()
{
	if (!_keypressed || (_activeChannel < 0)) return;
	switch (_activeMode)
	{
	case(PLAY) :
		playChannel(_activeChannel);
		break;

		//TODO:CASE(STOP)
	case(LOOP) :
		toggleLoopChannel(_activeChannel);
		break;
	}

	_keypressed = false;

}

void Looper::playChannel(const int & n)
{
	if (n < 0 || n> _channels.size())
		throw new std::exception("CHANNEL NUMBER OUT OF CHANNEL INDEX");

	_channels[n]->playSound();
}

void Looper::toggleLoopChannel(const int & n)
{
	_channels[n]->setLooping(!_channels[n]->getLooping());
}

void Looper::printHUD()
{
	printf("Active Mode: %d\n", _activeMode);
	printf("Active Channel %d\n", _activeChannel);
	printf("-------------------------------------------------------------------\n");
	printf("Channels can be selected with numbers 1-8\n");
	printf("LOOPER MODES:\n");
	printf("P: PLAY MODE. Select a Channel and then P to play it\n");
	printf("L: LOOP MODE. Select a Channel and then L to toggle its loop state\n");
	printf("Channels can be selected with numbers 1-8\n");
	printf("-------------------------------------------------------------------\n");
	
	printf("Loaded file: %s\n", dropped_filedir);

}

void Looper::processDrop()
{
	SDL_Event e;
	SDL_PollEvent(&e);

	if (e.type == SDL_DROPFILE) 
	{
		dropped_filedir = e.drop.file;
		
		//_activeChannel = 8;
		/*
		if (_channels[_activeChannel] != nullptr)
			_channels[_activeChannel]->loadFile(dropped_filedir);
		else{*/

		LooperChannel* aux = new LooperChannel(_system, numChannels);
		aux->loadFile(dropped_filedir);
		aux->setRectX(numChannels * 100);
		_channels.push_back(aux);	
		numChannels++;
			
		//}
	}
}

bool Looper::run() 
{
	system("CLS");

	processKeys();
	processDrop();
	processState();
	render();

	FMOD_RESULT res = _system->update();
	try
	{
		ERRCHECK(res);
	}
	catch (const std::exception& e)
	{
		std::cout << "An exception has ocurred in run(): " << e.what() << std::endl;
		system("PAUSE");
		return false;
	}
	printHUD();
	return true;
}

