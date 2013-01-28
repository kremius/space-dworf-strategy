#pragma once

#include <SDL.h>
#include <SDL_mixer.h>

#include <map>
#include <string>

namespace AUDIO_CONST
{
    const int audio_rate = 22050;
    const Uint16 audio_format = AUDIO_S16; 
    const int audio_channels = 2;
    const int audio_buffers = 8192;
}

class Audio
{
public:
    Audio();
    bool Play(const std::string& name, int volume = MIX_MAX_VOLUME, int loop = 0);
    ~Audio();
private:
    int amount_channels_;
    std::map<std::string, Mix_Chunk*> sounds_;
};

Audio* GetAudio();

namespace helpers
{
    void InitAudio(Audio* audio);
}