#ifndef INCLUDED_ENGINE_LOCAL_SYSTEM_H
#define INCLUDED_ENGINE_LOCAL_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "core/program_state.h"
#include "soldier_properties_ready_event.h"
#include "input/keyboard.h"

namespace engine {

class LocalSystem : public System
{
public:
    DEFINE_SYSTEM_BASE(LocalSystem)
    LocalSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
    core::ProgramState& mProgramState;
    ModelValue mLocalStartModel;
    void OnLocalStart();
    AutoReg mOnSoldierPropertiesReady;
    void OnSoldierPropertiesReady( SoldierPropertiesReadyEvent const& Evt );
    Opt<engine::KeyboardSystem> mKeyboard;
};

} // namespace engine

#endif//INCLUDED_ENGINE_LOCAL_SYSTEM_H
