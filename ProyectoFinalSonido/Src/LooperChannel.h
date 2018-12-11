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
	LooperChannel(FMOD::System * syst, const char * path, const short & channelNo);
	virtual ~LooperChannel();
	void loadSound(FMOD::Sound *, const bool & looping = false);
	void loadSound(const char * soundName,const bool & looping = false);
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

	SDL_Rect rect;

};

