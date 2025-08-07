
#include "Sound.hpp"

#include <SDL3/SDL.h>

#include <vector>
//#include <SDL3/SDL.h>

static SDL_AudioStream* g_pAudioStream = nullptr;
static uint8_t* g_pWavData = nullptr;
static SDL_AudioDeviceID g_AudioDevice = 0;

/* things that are playing sound (the audiostream itself, plus the original data, so we can refill to loop. */
typedef struct Sound 
{
	Uint8* wav_data = nullptr;
	Uint32 wav_data_len = 0;
	SDL_AudioStream* stream = nullptr;
} Sound;

static void SDLCALL FreeTheAudioStream(void* userdata, SDL_AudioStream* astream, int additional_amount, int total_amount)
{
//	SDL_ClearAudioStream(g_pAudioStream);
//	SDL_free(g_pWavData);
//	SDL_DestroyAudioStream(g_pAudioStream);
//	g_pAudioStream = nullptr;
//	g_pWavData = nullptr;
}

cAudioDevices::cAudioDevices()
{
	int count = 0;

	SDL_AudioDeviceID* ids = SDL_GetAudioPlaybackDevices(&count);

	if (ids && (count > 0))
	{
		for (int i = 0; i < count; ++i)
		{
			sAudioDevice device;
			device.audio_device_ID = ids[i];
			device.audio_device_name = SDL_GetAudioDeviceName(device.audio_device_ID);

			mAudioDevices.push_back(device);
		}
	}

	if (ids)
		SDL_free(ids);
}

cAudioDevices::~cAudioDevices()
{}

const std::vector<sAudioDevice>& cAudioDevices::getAudioDevices() const
{
	return mAudioDevices;
}


cSound::cSound()
{
	int count = 0;

	SDL_AudioDeviceID* ids = SDL_GetAudioPlaybackDevices(&count);

	if (ids && (count > 0))
	{
		for (int i = 0; i < count; ++i)
		{
			sAudioDevice device;
			device.audio_device_ID = ids[i];
			device.audio_device_name = SDL_GetAudioDeviceName(device.audio_device_ID);

			mAudioDevices.push_back(device);
		}
	}

	if (ids)
		SDL_free(ids);
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

bool cSound::setPlaybackDevice(SDL_AudioDeviceID id)
{
	for (const auto& device : mAudioDevices)
	{
		if (device.audio_device_ID == id)
		{
			mAudioDeviceID = id;
			return true;
		}
	}

	return false;
}

// SDL_SetAudioDeviceGain(SDL_AudioDeviceID devid, float gain);

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

bool testSound(SDL_AudioDeviceID id, const std::string& filename)
{
	if ((g_pAudioStream) || (g_pWavData))
		return false;

	/* open the default audio device in whatever format it prefers; our audio streams will adjust to it. */
	g_AudioDevice = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
	if (g_AudioDevice == 0)
	{
		SDL_Log("Couldn't open audio device: %s", SDL_GetError());
		return false;
	}

	float gain = SDL_GetAudioDeviceGain(g_AudioDevice);


	bool retval = false;
	SDL_AudioSpec spec;
	char* wav_path = NULL;
	Sound sound;

	/* Load the .wav files from wherever the app is being run from. */
	SDL_asprintf(&wav_path, "%s%s", SDL_GetBasePath(), filename.c_str());  /* allocate a string of the full file path */
	if (!SDL_LoadWAV(wav_path, &spec, &sound.wav_data, &sound.wav_data_len)) {
		SDL_Log("Couldn't load .wav file: %s", SDL_GetError());
		return false;
	}

	/* Create an audio stream. Set the source format to the wav's format (what
	   we'll input), leave the dest format NULL here (it'll change to what the
	   device wants once we bind it). */
	sound.stream = SDL_CreateAudioStream(&spec, NULL);
	if (!sound.stream) 
	{
		SDL_Log("Couldn't create audio stream: %s", SDL_GetError());
	}
	else if (!SDL_BindAudioStream(g_AudioDevice, sound.stream)) {  /* once bound, it'll start playing when there is data available! */
		SDL_Log("Failed to bind '%s' stream to device: %s", filename.c_str(), SDL_GetError());
	}
	else {
		retval = true;  /* success! */
	}

	SDL_free(wav_path);  /* done with this string. */

	auto result = SDL_PutAudioStreamData(sound.stream, sound.wav_data, (int)sound.wav_data_len);
	if (!result)
	{
		std::string msg = SDL_GetError();
	}

	result = SDL_ResumeAudioStreamDevice(sound.stream);
	if (!result)
	{
		std::string msg = SDL_GetError();
	}

#if 0
	SDL_AudioSpec spec;
	char* wav_path = nullptr;
	uint32_t wav_data_len = 0;

	SDL_asprintf(&wav_path, filename.c_str());
	if (!SDL_LoadWAV(wav_path, &spec, &g_pWavData, &wav_data_len))
	{
		SDL_Log("Couldn't load .wav file: %s", SDL_GetError());
		return false;
	}

	SDL_free(wav_path);  /* done with this string. */

	float gain = SDL_GetAudioDeviceGain(id);

	/* Create our audio stream in the same format as the .wav file. It'll convert to what the audio hardware wants. */
	g_pAudioStream = SDL_OpenAudioDeviceStream(id, &spec, FreeTheAudioStream, NULL);

	if (!g_pAudioStream)
	{
		SDL_Log("Couldn't create audio stream: %s", SDL_GetError());
		SDL_free(g_pWavData);
		g_pWavData = nullptr;
		return false;
	}
	auto result = SDL_PutAudioStreamData(g_pAudioStream, g_pWavData, wav_data_len);
	if (!result)
	{
		std::string msg = SDL_GetError();
	}

	result = SDL_ResumeAudioStreamDevice(g_pAudioStream);
	if (!result)
	{
		std::string msg = SDL_GetError();
	}
#endif

	return true;
}


