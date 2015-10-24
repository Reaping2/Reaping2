#ifndef INCLUDED_ENGINE_CTF_SOLDIER_SPAWN_SYSTEM_H
#define INCLUDED_ENGINE_CTF_SOLDIER_SPAWN_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "core/actor.h"
#include "core/program_state.h"
#include "core/actor_factory.h"
#include "core/ctf_program_state.h"
#include "core/revive_event.h"

namespace engine {
namespace ctf {
class CtfSoldierSpawnSystem : public System
{
public:
    DEFINE_SYSTEM_BASE(CtfSoldierSpawnSystem)
    CtfSoldierSpawnSystem();
    static Opt<CtfSoldierSpawnSystem> Get();
    std::auto_ptr<Actor> Spawn(::core::ClientData& clientData, Team::Type team);
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
    ActorFactory& mActorFactory;
    int32_t mPlayerAutoId;
    AutoReg mOnRevive;
    void OnRevive(core::ReviveEvent const& Evt);
};
} // namespace ctf
} // namespace engine

#endif//INCLUDED_ENGINE_CTF_SOLDIER_SPAWN_SYSTEM_H

//command:  "classgenerator.exe" -c "ctf_soldier_spawn_system" -g "system" -n "engine"
