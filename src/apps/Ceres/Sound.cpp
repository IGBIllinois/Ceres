
#include "Sound.hpp"

#include <SDL3/SDL.h>

#include <vector>
//#include <SDL3/SDL.h>

cSound::cSound()
{
}

cSound::~cSound()
{
	if (mWavData)
	{
		SDL_free(mWavData);
		mWavData = nullptr;
		mWavDataLen = 0;
	}
}


bool cSound::open(const std::string& filename)
{
	SDL_AudioSpec spec;
	char* wav_path = nullptr;

//	SDL_asprintf(&wav_path, filename.c_str(), SDL_GetBasePath());

	SDL_asprintf(&wav_path, filename.c_str());
	if (!SDL_LoadWAV(wav_path, &spec, &mWavData, &mWavDataLen))
	{
		SDL_Log("Couldn't load .wav file: %s", SDL_GetError());
		return false;
	}

	SDL_free(wav_path);  /* done with this string. */

	/* Create our audio stream in the same format as the .wav file. It'll convert to what the audio hardware wants. */
	mpAudioStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);
	if (!mpAudioStream)
	{
		SDL_Log("Couldn't create audio stream: %s", SDL_GetError());
		return false;
	}

	return true;
}

void cSound::close()
{
	if (mpAudioStream)
		SDL_ClearAudioStream(mpAudioStream);

	if (mWavData)
	{
		SDL_free(mWavData);
		mWavData = nullptr;
		mWavDataLen = 0;
	}

	if (mpAudioStream)
	{
		SDL_DestroyAudioStream(mpAudioStream);
		mpAudioStream = nullptr;
	}
}

bool cSound::is_open()
{
	return mpAudioStream && mWavData;
}

void cSound::play()
{
	/* SDL_OpenAudioDeviceStream starts the device paused. You have to tell it to start! */
	if (mpAudioStream)
	{

		auto result = SDL_PutAudioStreamData(mpAudioStream, mWavData, mWavDataLen);
		if (!result)
		{
			std::string msg = SDL_GetError();
		}

		result = SDL_ResumeAudioStreamDevice(mpAudioStream);
		if (!result)
		{
			std::string msg = SDL_GetError();
		}
	}

}

