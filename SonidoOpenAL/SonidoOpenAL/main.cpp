#include <al.h>
#include <alc.h>
#include <alut.h>
#include <iostream>

int main()
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

	ALuint buffer = alutCreateBufferFromFile("../SonidoOpenAL/Sonidos/motores/1.wav");	if (alutGetError() != AL_NO_ERROR) {		std::cout << alutGetError() << std::endl;		std::getchar();	}	ALuint source;	alGenSources(1, &source);	/*	// Para cambiar el pitch
	// posicion y velociddad: vectores en coordenadas cartesianas
	ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };
	ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };
	alSourcefv(Source, AL_POSITION, SourcePos);
	alSourcefv(Source, AL_VELOCITY, SourceVel);
	alSourcef (Source, AL_PITCH, 1.0 ); // frec de reproduccion
	alSourcef (Source, AL_GAIN, 1.0 ); // ganancia de la fuente
	alSourcei (Source, AL_LOOPING, true); // para repr. ciclica	*/	alSourcei(source, AL_BUFFER, buffer);	alSourcePlay(source);		alutSleep(3);	alutExit();

	return 0;
}