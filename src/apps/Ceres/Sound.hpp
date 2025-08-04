
#pragma once

#include <SDL3/SDL_audio.h>

#include <cstdint>
#include <string>
#include <vector>

struct sAudioDevice
{
    SDL_AudioDeviceID audio_device_ID = SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK;
    std::string audio_device_name;
};

class cAudioDevices
{
public:
    cAudioDevices();
    ~cAudioDevices();

    const std::vector<sAudioDevice>& getAudioDevices() const;

private:
    std::vector<sAudioDevice> mAudioDevices;
};


class cSound
{
public:
    cSound();
    ~cSound();

    bool setPlaybackDevice(SDL_AudioDeviceID id);

    bool open(const std::string& filename);
    void close();

    bool is_open();

    void play();

    bool test(SDL_AudioDeviceID id, const std::string& filename);

private:
    SDL_AudioStream* mpAudioStream = nullptr;
    uint8_t* mWavData = nullptr;
    uint32_t mWavDataLen = 0;

    SDL_AudioDeviceID mAudioDeviceID = SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK;

    std::vector<sAudioDevice> mAudioDevices;
};

