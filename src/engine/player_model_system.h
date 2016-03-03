#ifndef INCLUDED_ENGINE_PLAYER_MODEL_SYSTEM_H
#define INCLUDED_ENGINE_PLAYER_MODEL_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "core/actor_event.h"
#include "core/program_state.h"

namespace engine {

class PlayerModelSystem : public System
{
public:
    DEFINE_SYSTEM_BASE(PlayerModelSystem)
    PlayerModelSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
    core::ProgramState& mProgramState;
    AutoReg mOnActorEvent;
    void OnActorEvent( ActorEvent const& Evt );
};

} // namespace engine

#endif//INCLUDED_ENGINE_PLAYER_MODEL_SYSTEM_H

//command:  "classgenerator" -g "system" -c "player_model_system"
