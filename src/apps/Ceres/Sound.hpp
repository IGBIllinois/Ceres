
#pragma once

#include <cstdint>
#include <string>

//Forwared Declarations
class SDL_AudioStream;

class cSound
{
public:
    cSound();
    ~cSound();

    bool open(const std::string& filename);
    void close();

    bool is_open();

    void play();

private:
    SDL_AudioStream* mpAudioStream = nullptr;
    uint8_t* mWavData = nullptr;
    uint32_t mWavDataLen = 0;

};

