#ifndef INCLUDED_ENGINE_MOUSE_ADAPTER_SYSTEM_H
#define INCLUDED_ENGINE_MOUSE_ADAPTER_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "mouse.h"
#include "input_system.h"
#include "core/program_state.h"

namespace engine {

class MouseAdapterSystem : public System
{
public:
    DEFINE_SYSTEM_BASE(MouseAdapterSystem)
    MouseAdapterSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
    Opt<MouseSystem> mMouse;
    double mX;
    double mY;
    AutoReg mMouseMoveId;
    void OnMouseMoveEvent( const WorldMouseMoveEvent& Event );

    Opt<InputSystem> mInputSystem;
    core::ProgramState& mProgramState;
};

} // namespace engine

#endif//INCLUDED_ENGINE_MOUSE_ADAPTER_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -c "mouse_adapter_system" -n "engine"
