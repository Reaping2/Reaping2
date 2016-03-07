#ifndef INCLUDED_ENGINE_SOLDIER_SPAWN_SYSTEM_H
#define INCLUDED_ENGINE_SOLDIER_SPAWN_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "core/program_state.h"
#include "core/actor.h"
#include "core/actor_factory.h"
#include "core/map/soldier_spawn_point_map_element_system.h"
#include "core/revive_event.h"

namespace engine {

class SoldierSpawnSystem : public System
{
public:
    DEFINE_SYSTEM_BASE( SoldierSpawnSystem )
    SoldierSpawnSystem();
    static Opt<SoldierSpawnSystem> Get();
    std::auto_ptr<Actor> Spawn( core::ClientData& clientData );
    std::auto_ptr<Actor> Spawn( core::ClientData& clientData, map::SpawnPoint spawnPoint );
    std::auto_ptr<Actor> Spawn( core::ClientData& clientData, map::SpawnPoint spawnPoint, std::auto_ptr<Actor> player );
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
    core::ProgramState& mProgramState;
    ActorFactory& mActorFactory;
    int32_t mPlayerAutoId;
    AutoReg mOnRevive;
    void OnRevive( core::ReviveEvent const& Evt );
};

} // namespace engine

#endif//INCLUDED_ENGINE_SOLDIER_SPAWN_SYSTEM_H

