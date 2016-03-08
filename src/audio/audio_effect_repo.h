#ifndef INCLUDED_AUDIO_AUDIO_EFFECT_PLAYER_H
#define INCLUDED_AUDIO_AUDIO_EFFECT_PLAYER_H

#include "platform/i_platform.h"
#include "audio_effect.h"

namespace audio {

class AudioEffectRepo : public Singleton<AudioEffectRepo>, public Repository<AudioEffect>
{
    AudioEffect mDefaultEffect;
    friend class Singleton<AudioEffectRepo>;
    AudioEffectRepo();
    void Load();
};

}

#endif//INCLUDED_AUDIO_AUDIO_EFFECT_PLAYER_H
