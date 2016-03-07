#ifndef INCLUDED_ENGINE_POINTER_TARGET_CONTROLLER_SUB_SYSTEM_H
#define INCLUDED_ENGINE_POINTER_TARGET_CONTROLLER_SUB_SYSTEM_H

#include "engine/sub_system.h"
#include "core/scene.h"
#include "core/program_state.h"
#include "engine/removed_actors_system.h"
#include "core/actor_factory.h"
using core::ProgramState;

namespace engine {

class PointerTargetControllerSubSystem : public SubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE( PointerTargetControllerSubSystem )
    PointerTargetControllerSubSystem();
protected:
    Opt<RemovedActorsSystem> mRemovedActorsSystem;
    virtual void Init();
    virtual void Update( Actor& actor, double DeltaTime );
private:
    Scene& mScene;
    ProgramState& mProgramState;
    ActorFactory& mActorFactory;
    int32_t mLaserPointerProjectileId;
};

} // namespace engine

#endif//INCLUDED_ENGINE_POINTER_TARGET_CONTROLLER_SUB_SYSTEM_H
