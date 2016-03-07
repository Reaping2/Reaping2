#ifndef INCLUDED_ENGINE_KEYBOARD_ADAPTER_SYSTEM_H
#define INCLUDED_ENGINE_KEYBOARD_ADAPTER_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "keyboard.h"
#include "core/program_state.h"
#include "input_system.h"

namespace engine {

class KeyboardAdapterSystem : public System
{
public:
    DEFINE_SYSTEM_BASE( KeyboardAdapterSystem )
    KeyboardAdapterSystem();
    enum MoveFlags
    {
        MF_Up       = 1 << 0,
        MF_Down     = 1 << 1,
        MF_Left     = 1 << 2,
        MF_Right    = 1 << 3,
    };
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
    Opt<KeyboardSystem> mKeyboard;
    Opt<InputSystem> mInputSystem;
    core::ProgramState& mProgramState;
};

} // namespace engine

#endif//INCLUDED_ENGINE_KEYBOARD_ADAPTER_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -c "keyboard_adapter_system" -n "engine"
