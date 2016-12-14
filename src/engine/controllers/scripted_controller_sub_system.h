#ifndef INCLUDED_ENGINE_SCRIPTED_CONTROLLER_SUB_SYSTEM_H
#define INCLUDED_ENGINE_SCRIPTED_CONTROLLER_SUB_SYSTEM_H

#include "engine/sub_system.h"
#include "core/scene.h"
#include "core/program_state.h"
using core::ProgramState;

namespace engine {

class ScriptedControllerSubSystem : public SubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE(ScriptedControllerSubSystem)
    ScriptedControllerSubSystem();
protected:
    virtual void Init();
    virtual void Update( Actor& actor, double DeltaTime );
private:
    Scene& mScene;
    ProgramState& mProgramState;
};

} // namespace engine

#endif//INCLUDED_ENGINE_SCRIPTED_CONTROLLER_SUB_SYSTEM_H

//command:  "classgenerator.exe" -g "controller_sub_system" -c "scripted_controller_sub_system" -t "scripted"
