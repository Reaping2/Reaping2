#ifndef INCLUDED_ENGINE_EXPLODE_ON_DEATH_SYSTEM_H
#define INCLUDED_ENGINE_EXPLODE_ON_DEATH_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "core/actor_factory.h"

namespace engine {

class ExplodeOnDeathSystem : public System
{
public:
    DEFINE_SYSTEM_BASE(ExplodeOnDeathSystem)
    ExplodeOnDeathSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );

    void SetProjectilePosition(Actor& projectile, Actor& actor);

private:
    Scene& mScene;
    ActorFactory& mActorFactory;
};

} // namespace engine

#endif//INCLUDED_ENGINE_EXPLODE_ON_DEATH_SYSTEM_H
