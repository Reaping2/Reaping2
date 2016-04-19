#include "platform/i_platform.h"
#include "mouse_adapter_system.h"
#include "core/i_position_component.h"
#include "core/actor.h"
#include "engine/engine.h"
#include "mouse.h"
#include "player_control_device.h"

namespace engine {

MouseAdapterSystem::MouseAdapterSystem()
    : mScene( Scene::Get() )
    , mProgramState( core::ProgramState::Get() )
    , mX( 0.0 )
    , mY( 0.0 )
{
}


void MouseAdapterSystem::Init()
{
    mMouse = Engine::Get().GetSystem<MouseSystem>();
    mInputSystem = InputSystem::Get();
    mMouseMoveId = EventServer<WorldMouseMoveEvent>::Get().Subscribe( boost::bind( &MouseAdapterSystem::OnMouseMoveEvent, this, _1 ) );
}


void MouseAdapterSystem::Update( double DeltaTime )
{
    Opt<Actor> actor( mScene.GetActor( mProgramState.mControlledActorGUID ) );
    if ( !actor.IsValid() )
    {
        return;
    }
    int32_t playerId = 1;
    static input::PlayerControlDevice& pcd( input::PlayerControlDevice::Get() );
    if( pcd.GetControlDevice( playerId ) != input::PlayerControlDevice::KeyboardAndMouse )
    {
        return;
    }
    InputState inputState = mInputSystem->GetInputState( playerId );

    Opt<IPositionComponent> actorPositionC = actor->Get<IPositionComponent>();
    inputState.mOrientation = atan2( mY - actorPositionC->GetY(), mX - actorPositionC->GetX() );

    if ( mMouse->IsButtonPressed( MouseSystem::Button_Left ) )
    {
        inputState.mShoot = true;
    }
    else if ( mMouse->IsButtonPressed( MouseSystem::Button_Right ) )
    {
        inputState.mShootAlt = true;
    }
    mInputSystem->SetInputState( playerId, inputState );
}

void MouseAdapterSystem::OnMouseMoveEvent( const WorldMouseMoveEvent& Event )
{
    mX = Event.Pos.x;
    mY = Event.Pos.y;
}


} // namespace engine

