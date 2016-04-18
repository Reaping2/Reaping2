#include "platform/i_platform.h"
#include "input_system.h"
#include "engine/engine.h"
#include "input_state_changed.h"

namespace engine {

InputSystem::InputSystem()
    : mScene( Scene::Get() )
{
}


void InputSystem::Init()
{
}


void InputSystem::Update( double DeltaTime )
{
    std::for_each( mInputStates.begin(), mInputStates.end(), []( InputStates::value_type& v ) { v.second.Reset(); } );
}

Opt<InputSystem> InputSystem::Get()
{
    return engine::Engine::Get().GetSystem<InputSystem>();
}

engine::InputState const& InputSystem::GetInputState( int32_t playerId )
{
    return mInputStates[ playerId ];
}

void InputSystem::SetInputState( int32_t playerId, InputState const& inputState )
{
    auto& storedState = mInputStates[ playerId ];
    if ( storedState != inputState )
    {
        storedState = inputState;
        EventServer<InputStateChangedEvent>::Get().SendEvent( InputStateChangedEvent( playerId, storedState ) );
    }
}


} // namespace engine

