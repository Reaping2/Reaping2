#ifndef INCLUDED_ENGINE_GUARD_CONTROLLER_SUB_SYSTEM_H
#define INCLUDED_ENGINE_GUARD_CONTROLLER_SUB_SYSTEM_H

#include "engine/sub_system.h"
#include "core/scene.h"
#include "core/program_state.h"
#include "core/i_target_holder_component.h"
#include "core/i_position_component.h"
using core::ProgramState;

namespace engine {

class GuardControllerSubSystem : public SubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE(GuardControllerSubSystem)
    GuardControllerSubSystem();
protected:
    virtual void Init();
    virtual void Update( Actor& actor, double DeltaTime );
private:
    Scene& mScene;
    ProgramState& mProgramState;
    void UpdateTarget( Actor& actor, Opt<ITargetHolderComponent> targetHolderC );
    int32_t GetDistanceSqr( Opt<IPositionComponent> positionC, Opt<IPositionComponent> otherPositionC );
};

} // namespace engine

#endif//INCLUDED_ENGINE_GUARD_CONTROLLER_SUB_SYSTEM_H

//command:  "classgenerator.exe" -g "controller_sub_system" -c "guard_controller_sub_system" -t "guard"
