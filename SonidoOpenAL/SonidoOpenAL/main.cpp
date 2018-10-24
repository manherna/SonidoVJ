#include <al.h>
#include <alc.h>
#include <alut.h>
#include <iostream>
#include <conio.h>

#include <time.h>

int ex2();

//Returns the int asociated to the buffer in which the file is loaded
/*
ALuint al_loadWavFile(char * toLoad) 
{
	
	ALuint Buffer;
	alGenBuffers(1, &Buffer); //num de buffers y puntero a ellos
	if (alGetError() != AL_NO_ERROR) return -1;
	// variables de retorno de la apertura de un wav
	ALenum format; ALsizei size; ALvoid* data; ALsizei freq; ALboolean loop;
	// cargamos datos (en un espacio intermedio, una especie de clipboard)
	alutLoadWAVFile(toLoad, &format, &data, &size, &freq, &loop);
	// los pasamos al buffer
	alBufferData(Buffer, format, data, size, freq);
	// descargamos datos del almacenamiento in

	return Buffer;
}
*/
int main()
{

	return ex2();
}
int ex2() 
{
	srand(time(NULL));

	alutInit(0, NULL);
	alGetError();
	ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };

	alListener3f(AL_POSITION, 0, 0, 1.0f);
	// check for errors
	alListener3f(AL_VELOCITY, 0, 0, 0);
	// check for errors

	// Esto es para cambiar la orientación del oyente.
	//alListenerfv(AL_ORIENTATION, listenerOri);

	ALuint battle = alutCreateBufferFromFile("../SonidoOpenAL/Sonidos/batalla/Battle.wav");
	ALuint gun1 = alutCreateBufferFromFile("../SonidoOpenAL/Sonidos/batalla/Gun1.wav");
	ALuint gun2 = alutCreateBufferFromFile("../SonidoOpenAL/Sonidos/batalla/Gun2.wav");
	if (alutGetError() != AL_NO_ERROR) {
		std::cout << alutGetError() << std::endl;
		std::getchar();
	}
	ALuint sources[3];
	alGenSources(3, sources);

	alSourcei(sources[0], AL_BUFFER, battle);
	alSourcei(sources[1], AL_BUFFER, gun1);
	alSourcei(sources[2], AL_BUFFER, gun2);

	//Reproducimos Batalla en Loop
	alSourcei(sources[0], AL_LOOPING, AL_TRUE);
	alSourcePlay(sources[0]);
	alSourcePlay(sources[1]);

	bool quit = false;

	while(true)
	{
		std::system("CLS");
		std::cout << "Quit with q" << std::endl;
		
	
		ALint playing1, playing2;
		alGetSourcei(sources[1], AL_SOURCE_STATE, &playing1);
		alGetSourcei(sources[2], AL_SOURCE_STATE, &playing2);

		
		if (playing1 != AL_PLAYING)
		{
			int mojonx = 1 + rand() % (5 - 1);
			int mojonz = 1 + rand() % (5 - 1);
			ALfloat SourcePos[] = { mojonx, 0.0, mojonz };
			alSourcefv(sources[1], AL_POSITION, SourcePos);
			alSourcePlay(sources[1]);
		}
		if (playing2 != AL_PLAYING)
		{
			int mojonx = 1 + rand() % (5 - 1);
			int mojonz = 1 + rand() % (5 - 1);
			ALfloat SourcePos[] = { mojonx, 0.0, mojonz };
			alSourcefv(sources[2], AL_POSITION, SourcePos);
			alSourcePlay(sources[2]);
		}
			
	}

	std::cout << "Closing... grasias vuelva pronto" << std::endl;
	for (int i = 0; i < 2; i++)
		alSourceStop(sources[i]);

	alutSleep(2);
	alutExit();

	return 0;
}





int ex1()
{	
	alutInit(0, NULL);
	alGetError();
	ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };

	alListener3f(AL_POSITION, 0, 0, 1.0f);
	// check for errors
	alListener3f(AL_VELOCITY, 0, 0, 0);
	// check for errors

	// Esto es para cambiar la orientación del oyente.
	//alListenerfv(AL_ORIENTATION, listenerOri);
	// check for errors

	ALuint buffer = alutCreateBufferFromFile("../SonidoOpenAL/Sonidos/motores/1_new.wav");
	ALuint buffer2 = alutCreateBufferFromFile("../SonidoOpenAL/Sonidos/motores/1_high.wav");
	if (alutGetError() != AL_NO_ERROR) {
		std::cout << alutGetError() << std::endl;
		std::getchar();
	}
	ALuint sources[2];
	alGenSources(2, sources);

	alSourcei(sources[0], AL_BUFFER, buffer);
	alSourcei(sources[1], AL_BUFFER, buffer2);

	alSourcePlay(sources[0]);
	alSourcei(sources[0], AL_LOOPING, AL_TRUE);
	alSourcei(sources[1], AL_LOOPING, AL_TRUE);

	float pitch = 1.0F;
	float pitch_alto = 1.0F;
	int key_code;
	bool alto = false;

	while (true) {
		
		std::system("CLS");
		std::cout << "Pitch change: d/a" << std::endl;
		std::cout << "Quit with: q" << std::endl;
		std::cout << "Pitch: " << pitch << std::endl;
		std::cout << "Pitch alto: " << pitch_alto << std::endl;

		key_code = _getch();
		//char option = std::getchar();

		if (key_code == 'a') {
			if (!alto) {
				pitch += 0.05;
				alSourcef(sources[0], AL_PITCH, pitch);
			}
			else {
				pitch_alto += 0.05;
				alSourcef(sources[1], AL_PITCH, pitch_alto);
			}
		}
		if (key_code == 'd')
		{
			if (!alto) {
				pitch -= 0.05;
				alSourcef(sources[0], AL_PITCH, pitch);
			}
			else {
				pitch_alto -= 0.05;
				alSourcef(sources[1], AL_PITCH, pitch_alto);
			}
			
		}
		else if (key_code == 'q')
			break;


		if (pitch > 4 && !alto) {
			alto = true;
			alSourcePlay(sources[1]);
			alSourceStop(sources[0]);
		}
		else if (alto  && pitch_alto < 1) {
			alto = false;
			pitch_alto = 1;
			alSourcePlay(sources[0]);
			alSourceStop(sources[1]);
		}



		/*
	// Para cambiar el pitch
	// posicion y velociddad: vectores en coordenadas cartesianas
	ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };
	ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };
	alSourcefv(Source, AL_POSITION, SourcePos);
	alSourcefv(Source, AL_VELOCITY, SourceVel);
	alSourcef (Source, AL_PITCH, 1.0 ); // frec de reproduccion
	alSourcef (Source, AL_GAIN, 1.0 ); // ganancia de la fuente
	alSourcei (Source, AL_LOOPING, true); // para repr. ciclica
	*/
		
		
	}

	std::cout << "Closing... grasias vuelva pronto" << std::endl;
	for (int i = 0; i < 2; i++)
		alSourceStop(sources[i]);

	alutSleep(2);
	alutExit();


	return 0;
}