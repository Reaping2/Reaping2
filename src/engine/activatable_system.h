#ifndef INCLUDED_ENGINE_ACTIVATABLE_SYSTEM_H
#define INCLUDED_ENGINE_ACTIVATABLE_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "core/program_state.h"

namespace engine {

class ActivatableSystem : public System
{
public:
    DEFINE_SYSTEM_BASE(ActivatableSystem)
    ActivatableSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene = Scene::Get();
    core::ProgramState& mProgramState = core::ProgramState::Get();
};

} // namespace engine

#endif//INCLUDED_ENGINE_ACTIVATABLE_SYSTEM_H

//command:  "classgenerator.exe" -g "component" -c "activatable_component" -m "bool-enabled bool-activated"
