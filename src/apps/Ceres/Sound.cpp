
#include "Sound.hpp"

#include <SDL3/SDL.h>

#include <vector>
//#include <SDL3/SDL.h>


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

bool cSound::test(SDL_AudioDeviceID id, const std::string& filename)
{
	SDL_AudioSpec spec;
	char* wav_path = nullptr;
	uint8_t* wav_data = nullptr;
	uint32_t wav_data_len = 0;

	SDL_asprintf(&wav_path, filename.c_str());
	if (!SDL_LoadWAV(wav_path, &spec, &wav_data, &wav_data_len))
	{
		SDL_Log("Couldn't load .wav file: %s", SDL_GetError());
		return false;
	}

	SDL_free(wav_path);  /* done with this string. */

	float gain = SDL_GetAudioDeviceGain(id);

	/* Create our audio stream in the same format as the .wav file. It'll convert to what the audio hardware wants. */
	SDL_AudioStream* pAudioStream = SDL_OpenAudioDeviceStream(id, &spec, NULL, NULL);
	if (!pAudioStream)
	{
		SDL_Log("Couldn't create audio stream: %s", SDL_GetError());
		return false;
	}
	auto result = SDL_PutAudioStreamData(pAudioStream, wav_data, wav_data_len);
	if (!result)
	{
		std::string msg = SDL_GetError();
	}

	result = SDL_ResumeAudioStreamDevice(pAudioStream);
	if (!result)
	{
		std::string msg = SDL_GetError();
	}

	SDL_ClearAudioStream(pAudioStream);
	SDL_free(wav_data);
	SDL_DestroyAudioStream(pAudioStream);

	return true;
}


