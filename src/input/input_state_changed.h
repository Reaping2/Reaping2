#ifndef INCLUDED_ENGINE_INPUT_STATE_CHANGED_H
#define INCLUDED_ENGINE_INPUT_STATE_CHANGED_H

#include "platform/event.h"
#include "input_state.h"

namespace engine {

struct InputStateChangedEvent : public platform::Event
{
    InputState const& mInputState;
    int32_t mPlayerId;
    InputStateChangedEvent( int32_t playerId, InputState const& inputState )
        : mInputState( inputState ), mPlayerId( playerId ) {}
};

} // namespace engine

#endif//INCLUDED_ENGINE_INPUT_STATE_CHANGED_H

//command:  "classgenerator.exe" -g "event" -c "input_state_changed" -m "InputState&-inputState"
