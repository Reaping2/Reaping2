#ifndef INCLUDED_ENGINE_DROP_ON_DEATH_SYSTEM_H
#define INCLUDED_ENGINE_DROP_ON_DEATH_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "core/actor_factory.h"

namespace engine {

class DropOnDeathSystem : public System
{
public:
    DEFINE_SYSTEM_BASE(DropOnDeathSystem)
    DropOnDeathSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
    ActorFactory& mActorFactory;

	AutoId Roll(int32_t n);
    AutoId RollNormalItem( int32_t n );
};

} // namespace engine

#endif//INCLUDED_ENGINE_DROP_ON_DEATH_SYSTEM_H
