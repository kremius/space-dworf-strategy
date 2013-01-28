#include "Audio.h"

#include <assert.h>

Audio* audio = nullptr;

Audio* GetAudio()
{
    assert(audio && "Audio is nullptr");
    return audio;
}

void helpers::InitAudio(Audio* new_audio)
{
    audio = new_audio;
}

Audio::Audio()
{
    SDL_Init(SDL_INIT_AUDIO);

    if (Mix_OpenAudio(AUDIO_CONST::audio_rate, 
                      AUDIO_CONST::audio_format, 
                      AUDIO_CONST::audio_channels, 
                      AUDIO_CONST::audio_buffers)) 
    {
        printf("Unable to open audio!\n");
        exit(1);
    }
    amount_channels_ = AUDIO_CONST::audio_channels;
}

bool Audio::Play(const std::string& name, int volume, int loop)
{
    if (sounds_.find(name) == sounds_.end())
        sounds_[name] = Mix_LoadWAV(name.c_str());
    assert(sounds_[name] && "Bad sound load");
    int play_channel = Mix_PlayChannel(-1, sounds_[name], loop);
    Mix_Volume(play_channel, volume);
    if (amount_channels_ == play_channel - 1)
        amount_channels_ = Mix_AllocateChannels(amount_channels_ * 2);
    return true;
}

Audio::~Audio()
{
    Mix_CloseAudio();
}