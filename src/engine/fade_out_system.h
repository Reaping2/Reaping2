#ifndef INCLUDED_ENGINE_FADE_OUT_SYSTEM_H
#define INCLUDED_ENGINE_FADE_OUT_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "core/actor_factory.h"

namespace engine {

class FadeOutSystem : public System
{
public:
    DEFINE_SYSTEM_BASE( FadeOutSystem )
    FadeOutSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
};

} // namespace engine

#endif//INCLUDED_ENGINE_FADE_OUT_SYSTEM_H
