#ifndef INCLUDED_ENGINE_SOLDIER_SPAWN_SYSTEM_H
#define INCLUDED_ENGINE_SOLDIER_SPAWN_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "core/program_state.h"
#include "core/actor.h"
#include "core/actor_factory.h"

namespace engine {

class SoldierSpawnSystem : public System
{
public:
    DEFINE_SYSTEM_BASE(SoldierSpawnSystem)
    SoldierSpawnSystem();
    static Opt<SoldierSpawnSystem> Get();
    std::auto_ptr<Actor> Spawn(core::ClientData& clientData);
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
    ActorFactory& mActorFactory;
    int32_t mPlayerAutoId;
};

} // namespace engine

#endif//INCLUDED_ENGINE_SOLDIER_SPAWN_SYSTEM_H

