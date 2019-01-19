#include "Looper.h"
#include <exception>
#include <iostream>
#include "Utilities.h"
#include <stdlib.h>
#include "Recorder.h"

SDL_Window* Looper::window = NULL;

Looper::Looper() {

}

Looper::~Looper() {

}

bool Looper::init()
{
	//SLD Window and libraries
	//Create window
	window = SDL_CreateWindow("Looper", 450, 175, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);
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

			//Initialize SDL_ttf
			if (TTF_Init() == -1)
			{
				printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());				
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

#if Debug
	//Carga de texturas
	emptySound = loadTexture("../Images/pistaVacia.png");
	fullSound = loadTexture("../Images/pistaFull.png");
	selector = loadTexture("../Images/seleccion.png");
	selecPos = { -100, 0, 100, 600 };
	recordingTexture = loadTexture("../Images/Recorder.png");
	recorderPos = { WIN_WIDTH - 120, 50, 70, 70 };
#else
	//Carga de texturas
	emptySound = loadTexture("../../Images/pistaVacia.png");
	fullSound = loadTexture("../../Images/pistaFull.png");
	selector = loadTexture("../../Images/seleccion.png");
	selecPos = { -100, 0, 100, 600 };
	recordingTexture = loadTexture("../../Images/Recorder.png");
	recorderPos = { WIN_WIDTH - 50, WIN_HEIGHT + 50, 70, 70 };
#endif
	
	if (emptySound == NULL || fullSound == NULL || selector == NULL || recordingTexture == NULL)
		printf("Failed loading textures\n");	

	Texto* aux = new Texto("Text0", renderer);
	aux->setPosition(20, 620);
	textos.push_back(aux);

	aux = new Texto("Text1", renderer);
	aux->setPosition(20, 636);
	textos.push_back(aux);

	aux = new Texto("Text2", renderer);
	aux->setPosition(20, 652);
	textos.push_back(aux);		

	aux = new Texto("Text3", renderer);
	aux->setPosition(300, 300);
	textos.push_back(aux);

	//_channels[_activeChannel]->loadSound("../Sounds/again.wav");
	_activeChannel = 0;	
	
	_activeMode = NOTHING;
	_lastActiveMode = NOTHING;
	_addMode = NOADD;
	_lastAddMode = NOADD;

	textos[0]->setString("P (play), X (remove), O (pitch), F (Flange), E (Echo), V (Volume), + and - (adjust), SPACE (no effect)");
	textos[3]->setString("Drop a file into the window to get started.");

	textos[1]->setString("Active Mode: ");	
	textos[2]->setString("Active Channel volume: ");

	Recorder::init(_system);
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

	for (int i = 0; i < _channels.size(); i++)
		delete _channels[i];

	for (int i = 0; i < textos.size(); i++)
		delete textos[i];

	for (int i = 0; i < textosCanciones.size(); i++)
		delete textosCanciones[i];	

	_channels.clear();
	textos.clear();
	textosCanciones.clear();

	SDL_free(emptySound);	
	SDL_free(fullSound);	
	SDL_free(selector);	
	SDL_free(recordingTexture);
	SDL_free(auxTexture);

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
}

//Utilizado para saber qué imprimir por pantalla
std::string Looper::getActiveMode()
{
	switch (_activeMode)
	{
	case NOTHING:
		return "Null";
		break;
	case PLAY:
		return "Play";
		break;
	case STOP:
		return "Pause";
		break;
	case LOOP:
		return "Loop";
		break;
	case VOLUME:
		return "Volume";
		break;
	case PITCH:
		if (_activeChannel < _channels.size() && _channels[_activeChannel])
			return "Pitch: " + std::to_string(_channels[_activeChannel]->getPitch());
		return "Pitch";
		break;	
	case RECORD:
		return "Record";
		break;
	case ECHO:
		if (_activeChannel < _channels.size() && _channels[_activeChannel] && _channels[_activeChannel]->getEchoActive()) 
			return "Echo (Active): " + std::to_string(_channels[_activeChannel]->getEcho());
		return "Echo";
		break;
	case FLANGE:
		return "Flange";
		break;
	default:	
		return "yay";
		break;
	}
}

void Looper::render()
{		
	SDL_RenderClear(renderer);

	auxTexture = nullptr;
	selecPos.x = _activeChannel * 100; //Selector

	//En funcion del estado de la pista, la textura sera "full" o "empty"
	for (int i = 0; i < _channels.size(); i++)
	{		
		if (_channels[i]->isPlaying()) auxTexture = fullSound;
		else auxTexture = emptySound;

		_channels[i]->setRectX(i*100); //Para que se coloquen segun su indice
		SDL_RenderCopy(renderer, auxTexture, NULL, &_channels[i]->getRect());
	}
	
	//Selector pista activa	
	SDL_RenderCopy(renderer, selector, NULL, &selecPos);
			
	//Textos parte inferior del programa con información
	for (int i = 0; i < textos.size(); i++){
		SDL_RenderCopy(renderer, textos[i]->getTexture(), NULL, &textos[i]->getRect());
	}

	for (int i = 0; i < textosCanciones.size(); i++){
		textosCanciones[i]->setRectX(i * 100+10); //Para que se coloquen segun su indice
		SDL_RenderCopy(renderer, textosCanciones[i]->getTexture(), NULL, &textosCanciones[i]->getRect());
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
		case (SDLK_x) :
			if (_activeChannel < _channels.size() && _channels[_activeChannel]) {
				deleteSound(_activeChannel);
				numChannels--;
				_activeMode = NOTHING; //Para que no afecte el modo activo al nuevo index del looper	
			}
			break;
		case (SDLK_r):
			_lastActiveMode = _activeMode;
			_activeMode = RECORD;
			break;
		case (SDLK_f):
			if (_activeChannel < _channels.size() && _channels[_activeChannel]) 
				_channels[_activeChannel]->toggleFlange();			
			_lastActiveMode = _activeMode;
			_activeMode = FLANGE;
			break;
		case (SDLK_e):
			if (_activeChannel < _channels.size() && _channels[_activeChannel])
				_channels[_activeChannel]->toggleEcho();
			_lastActiveMode = _activeMode;
			_activeMode = ECHO;
			break;
		case(SDLK_ESCAPE) :
			release();
			exit(-1);
			break;
		default:
			_keypressed = false;
			break;
		}
	}
}

void Looper::deleteSound(int n)
{	
	//Apartado sonoro
	_channels[n]->stopSound();	
	delete _channels[n];
	_channels.erase(_channels.begin() + n );

	//Apartado visual	
	delete textosCanciones[n];
	textosCanciones.erase(textosCanciones.begin() + n);
}

void Looper::processState()
{
	if (!_keypressed || (_activeChannel < 0)) return;

	float echo = 0;
	if (_activeChannel < _channels.size() && _channels[_activeChannel])
		 echo = _channels[_activeChannel]->getEcho();

	switch (_activeMode)
	{
	case(PLAY) :
		if (_activeChannel < _channels.size() && _channels[_activeChannel])
			playChannel(_activeChannel);
		break;
	case RECORD:
		if (Recorder::isRecording()) 
		{
			FMOD::Sound* record = Recorder::stopRecording();
			LooperChannel * nuevo = new LooperChannel(_system, numChannels);
			nuevo->loadSound(record, false);
			_channels.push_back(nuevo);
			_channels[numChannels]->playSound();

			Texto* tess = new Texto("Grabacion", renderer);
			tess->setPosition((numChannels * 100) + 10, 565);
			textosCanciones.push_back(tess);			
			numChannels++;
			_activeMode = NOTHING;
		}
		else {
			if (_channels.size() < 9) { //Si hay pistas disponibles
				Recorder::startRecording();
				_activeMode = NOTHING; //Para que no vuelva a entrar siguiente ciclo
			}
		}

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
	case(ECHO):		
		if (_lastAddMode != _addMode)
			if (_addMode == ADD)
				_channels[_activeChannel]->setEcho(echo + 10);
			else if (_addMode = REMOVE)
				_channels[_activeChannel]->setEcho(echo - 10);
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

	//Textura de recorder
	if (Recorder::isRecording())
		SDL_RenderCopy(renderer, recordingTexture, NULL, &recorderPos);
	
	//Textos	
	if (Recorder::isRecording())
		textos[1]->setString("Active Mode: Recording");

	activetext = getActiveMode();
	textos[1]->setString("Active Mode: " + activetext);

	short volumen = 0;
	if (_activeChannel < _channels.size() && _channels[_activeChannel])
		volumen = _channels[_activeChannel]->getVolume() * 100;

	textos[2]->setString("Active Channel volume: " + std::to_string(volumen));
}

void Looper::playChannel(const int & n)
{
	if (n < 0 || n > _channels.size())
		throw new std::exception("CHANNEL NUMBER OUT OF CHANNEL INDEX");

	if (_channels[n] != nullptr) {
		_channels[n]->playSound();		
	}
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
		textos[3]->setString(" "); //Ya no es necesario indicaciones
		dropped_filedir = e.drop.file;		

		std::string s = getTitle();

		Texto* tess = new Texto(s, renderer);
		tess->setPosition((numChannels*100)+10, 565);
		textosCanciones.push_back(tess);

		LooperChannel* aux = new LooperChannel(_system, numChannels);
		aux->loadFile(dropped_filedir);		
		_channels.push_back(aux);	
		numChannels++;		
	}
}

std::string Looper::getTitle()
{
	//Get the track title
	std::string s = dropped_filedir;
	std::string delimiter = "-";	

	size_t pos = 0;
	std::string token;

	while ((pos = s.find(delimiter)) != std::string::npos) {
		token = s.substr(0, pos);		
		s.erase(0, pos + delimiter.length());
	}	

	delimiter = "\\";	
	pos = 0;

	while ((pos = s.find(delimiter)) != std::string::npos) {
		token = s.substr(0, pos);		
		s.erase(0, pos + delimiter.length());
	}
	
	return s;
}

bool Looper::run() 
{
#if Debug
	//system("CLS");
#endif

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
#if Debug
	//printHUD();
#endif
	return true;
}




