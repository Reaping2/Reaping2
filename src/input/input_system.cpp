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


void InputSystem::Update(double DeltaTime)
{
    mInputState.Reset();
}

Opt<InputSystem> InputSystem::Get()
{
    return engine::Engine::Get().GetSystem<InputSystem>();
}

engine::InputState const& InputSystem::GetInputState() const
{
    return mInputState;
}

void InputSystem::SetInputState(InputState const& inputState)
{
    if (mInputState!=inputState)
    {
        mInputState=inputState;
        EventServer<InputStateChangedEvent>::Get().SendEvent(InputStateChangedEvent(mInputState));
    }
}


} // namespace engine

