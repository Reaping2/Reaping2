#ifndef INCLUDED_ENGINE_INPUT_SYSTEM_H
#define INCLUDED_ENGINE_INPUT_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "input_state.h"
#include <map>

namespace engine {

class InputSystem : public System
{
public:
    DEFINE_SYSTEM_BASE( InputSystem )
    InputSystem();
    static Opt<InputSystem> Get();
    InputState const& GetInputState( int32_t playerId );    //use this to access InputState.
    void SetInputState( int32_t playerId, InputState const& inputState );  //use this to modify InputState.
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
    typedef std::map<int32_t, InputState> InputStates;
    InputStates mInputStates;
};

} // namespace engine

#endif//INCLUDED_ENGINE_INPUT_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -c "input_system" -n "engine"
