#ifndef INCLUDED_AUDIO_AUDIO_SYSTEM_H
#define INCLUDED_AUDIO_AUDIO_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "core/actor_event.h"

namespace audio {

class AudioSystem : public engine::System
{
public:
    DEFINE_SYSTEM_BASE( AudioSystem )
    AudioSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
};

} // namespace audio

#endif//INCLUDED_AUDIO_AUDIO_SYSTEM_H

//command:  "../../build/tools/classgenerator/classgenerator" -g "system" -c "audio_system" -n "audio" -t "audible"
