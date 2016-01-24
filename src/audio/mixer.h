#ifndef INCLUDED_AUDIO_MIXER_H
#define INCLUDED_AUDIO_MIXER_H

#include "i_audio.h"

class Mixer
{
    float mAudioTypeRelativeWeights[audio::NumTypes];
public:
    Mixer();
    void Mix( AudioBuffer& Dest, AudioFiles_t& Files, size_t const Size );
};

#endif//INCLUDED_AUDIO_MIXER_H
