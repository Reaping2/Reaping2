#ifndef INCLUDED_ENGINE_INPUT_SYSTEM_H
#define INCLUDED_ENGINE_INPUT_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "input_state.h"

namespace engine {

class InputSystem : public System
{
public:
    DEFINE_SYSTEM_BASE( InputSystem )
    InputSystem();
    static Opt<InputSystem> Get();
    InputState const& GetInputState() const;    //use this to modify or access InputState.
    void SetInputState( InputState const& inputState );  //use this to modify InputState.
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
    InputState mInputState;
};

} // namespace engine

#endif//INCLUDED_ENGINE_INPUT_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -c "input_system" -n "engine"
