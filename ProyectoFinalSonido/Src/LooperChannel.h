#pragma once
#include <fmod.h>
#include <fmod.hpp>
#include <SDL.h>

class Ligma;

class LooperChannel
{
	struct channelAttr
	{
		float volume;
		float pitch;
		bool loop;
		Ligma * balls;
	};

public:
	LooperChannel();
	LooperChannel(FMOD::System * syst, const short & channelNo);	
	virtual ~LooperChannel();
	void release();

	void loadSound(FMOD::Sound *, const bool & looping = false);
	void loadFile(const char * soundName, const bool & looping = false);

	void playSound();
	void stopSound();
	void pauseSound();
	bool isPlaying();
	short getChannelNumber();

	float getPitch();
	void setPitch(const float & p);

	void setLooping(const bool & loop);
	bool getLooping();

	float getVolume();
	void setVolume(const float & v);

	void toggleEcho();
	void setEcho(float n);
	float getEcho() { return echo; }

	void toggleFlange();

	//SDL
	void setRectX(int n){ rect.x = n; }
	SDL_Rect getRect(){ return rect; }

private:
	void setChannelAttributes(FMOD::Channel * ch);
	FMOD::System * _system;
	FMOD::Sound * _sound;
	FMOD::Channel * _channel;
	channelAttr _attr;
	short _channelNo;

	//Efectos	
	FMOD::DSP *dsp_echo = 0;
	float echo = 50.0f;
	FMOD::DSP *dsp_flange = 0;	

	SDL_Rect rect;
};

