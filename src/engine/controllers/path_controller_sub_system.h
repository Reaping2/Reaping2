#ifndef INCLUDED_ENGINE_PATH_CONTROLLER_SUB_SYSTEM_H
#define INCLUDED_ENGINE_PATH_CONTROLLER_SUB_SYSTEM_H

#include "engine/sub_system.h"
#include "core/scene.h"
#include "core/program_state.h"
#include "core/i_target_holder_component.h"
#include "engine/path_system.h"
using core::ProgramState;

namespace engine {

class PathControllerSubSystem : public SubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE(PathControllerSubSystem)
    PathControllerSubSystem();
protected:
    virtual void Init();
    virtual void Update( Actor& actor, double DeltaTime );
private:
    Scene& mScene;
    ProgramState& mProgramState;
    void UpdateTarget( Actor& actor, Opt<ITargetHolderComponent> targetHolderC );
    Opt<engine::path::PathSystem> mPathSystem;
};

} // namespace engine

#endif//INCLUDED_ENGINE_PATH_CONTROLLER_SUB_SYSTEM_H

//command:  "classgenerator.exe" -g "controller_sub_system" -c "path_controller_sub_system" -t "path"
