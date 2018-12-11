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
	//SLD Window and libraries

	//Create window
	window = SDL_CreateWindow("Looper", 300, 300, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
	}
	else
	{
		//Create renderer for window
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (renderer == NULL)
		{
			printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Initialize renderer color
			SDL_SetRenderDrawColor(renderer, 0x00, 0x30, 0x66, 0xFF);

			//Initialize PNG loading
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) & imgFlags))
			{
				printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
			}
		}
	}

	//FMOD
	FMOD_RESULT res;
	try {
		res = System_Create(&_system);
		ERRCHECK(res);
		res = _system->init(128, FMOD_INIT_NORMAL, 0);
		ERRCHECK(res);		
	}
	catch (std::exception & e){
		std::cout << "An exception has ocurred: " << e.what() << std::endl;
		system("STOP");
		return false;
	}
	_activeChannel = -1;

	//Carga de texturas
	emptySound = loadTexture("../Images/pistaVacia.png");
	fullSound = loadTexture("../Images/pistaFull.png");

	if (emptySound == NULL || fullSound == NULL)
		printf("Failed loading textures\n");
		
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

	//_channels[_activeChannel]->loadSound("again.wav");
	_activeChannel = 0;
	
	
	_activeMode = NOTHING;
	_lastActiveMode = NOTHING;
	_addMode = NOADD;
	_lastAddMode = NOADD;

	return true;
}

SDL_Texture* Looper::loadTexture(std::string path)
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
		system("STOP");
		return;
	}
	//TODO: Release SDL
}

void Looper::render()
{		
	SDL_RenderClear(renderer);
	SDL_Texture* aux = nullptr;

	//En funcion del estado de la pista, la textura sera "full" o "empty"
	for (int i = 0; i < _channels.size(); i++)
	{		
		if (_channels[i]->isPlaying())aux = fullSound;
		else aux = emptySound;

		_channels[i]->setRectX(i*100); //Para que se coloquen segun su indice
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
		case(SDLK_PLUS):
			_lastAddMode = _addMode;
			_addMode = ADD;
			break;
		case(SDLK_MINUS):
			_lastAddMode = _addMode;
			_addMode = REMOVE;
			break;

		//This cases are to clean the channel and
		//State selection
		case (SDLK_l):
			_lastActiveMode = _activeMode;
			_activeMode = LOOP;
			break;
		case (SDLK_n) :
			_lastActiveMode = _activeMode;
			_activeMode = NOTHING;
			break;
		case(SDLK_SPACE):
			_lastActiveMode = _activeMode;
			_activeMode = NOTHING;
			_activeChannel = -1;
			break;
		case (SDLK_r) :
			deleteSound(_activeChannel);
			numChannels--;
			_activeMode = NOTHING; //Para que no afecte el modo activo al nuevo index del looper			
			break;
		default:
			_keypressed = false;
			break;
		}
	}
}

void Looper::deleteSound(int n)
{	
	_channels[n]->stopSound();
	_channels.erase(_channels.begin() + n );	
}

void Looper::processState()
{
	if (!_keypressed || (_activeChannel < 0)) return;
	switch (_activeMode)
	{
	case(PLAY) :
		if (_activeChannel < _channels.size() && _channels[_activeChannel])
			playChannel(_activeChannel);
		break;
	case(STOP) :	
		if (_activeChannel < _channels.size() && _channels[_activeChannel])
			pauseChannel(_activeChannel);
		break;
	case(LOOP):
		if (_activeChannel < _channels.size() && _channels[_activeChannel])
			toggleLoopChannel(_activeChannel);
		break;
	case(VOLUME):
		if (_lastAddMode != _addMode)
			if (_addMode == ADD) 
				_channels[_activeChannel]->setVolume(_channels[_activeChannel]->getVolume() + 0.1f);
			else if(_addMode = REMOVE)
				_channels[_activeChannel]->setVolume(_channels[_activeChannel]->getVolume() - 0.1f);
		_addMode = NOADD;
	case(PITCH):
		if (_lastAddMode != _addMode)
			if (_addMode == ADD)
				_channels[_activeChannel]->setPitch(_channels[_activeChannel]->getPitch() + 0.1f);
			else if (_addMode = REMOVE)
				_channels[_activeChannel]->setPitch(_channels[_activeChannel]->getPitch() - 0.1f);
		_addMode = NOADD;
	}
	_keypressed = false;
}

void Looper::playChannel(const int & n)
{
	if (n < 0 || n > _channels.size())
		throw new std::exception("CHANNEL NUMBER OUT OF CHANNEL INDEX");

	if (_channels[n] != nullptr)
		_channels[n]->playSound();
}

void Looper::pauseChannel(const int & nc)
{
	_channels[nc]->pauseSound();
}

void Looper::toggleLoopChannel(const int & n)
{
	_channels[n]->setLooping(!_channels[n]->getLooping());
}

void Looper::printHUD()
{
	printf("Active Mode: %d\n", _activeMode);
	printf("Active Channel %d\n", _activeChannel);
	if (_activeChannel >= 0 && _activeChannel < _channels.size()) {
		printf("Active Channel volume %f\n", _channels[_activeChannel]->getVolume());
		printf("Active Channel pitch %f\n", _channels[_activeChannel]->getPitch());
	}
		printf("-------------------------------------------------------------------\n");
	printf("Channels can be selected with numbers 1-8\n");
	printf("LOOPER MODES:\n");
	printf("P: PLAY MODE. Select a Channel and then P to play it\n");
	printf("L: LOOP MODE. Select a Channel and then L to toggle its loop state\n");
	printf("S: STOP MODE. Select a Channel and then P to stop a sound from playing\n");
	
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

		LooperChannel* aux = new LooperChannel(_system, numChannels);
		aux->loadFile(dropped_filedir);		
		_channels.push_back(aux);	
		numChannels++;		
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
		system("STOP");
		return false;
	}
	printHUD();
	return true;
}

