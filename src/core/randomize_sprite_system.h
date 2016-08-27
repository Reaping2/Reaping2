#ifndef INCLUDED_ENGINE_RANDOMIZE_SPRITE_SYSTEM_H
#define INCLUDED_ENGINE_RANDOMIZE_SPRITE_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "actor_event.h"

namespace engine {

class RandomizeSpriteSystem : public System
{
public:
    DEFINE_SYSTEM_BASE(RandomizeSpriteSystem)
    RandomizeSpriteSystem();
    static int32_t GetRandomIndex( std::vector<int32_t> const& randomSprites );
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
    AutoReg mOnActor;
    void OnActorEvent(::ActorEvent const& Evt);
};

} // namespace engine

#endif//INCLUDED_ENGINE_RANDOMIZE_SPRITE_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -c "randomize_sprite_system" -e -actor
