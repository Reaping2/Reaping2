#include "platform/i_platform.h"
#include "engine/controllers/player_controller_system.h"
#include "engine/engine.h"
#include "platform/auto_id.h"
#include "input/keyboard.h"
#include "core/player_controller_component.h"
#include "core/i_move_component.h"
#include "core/i_position_component.h"

namespace engine {

PlayerControllerSubSystem::PlayerControllerSubSystem()
    : mMouse( Mouse::Get() )
    , mScene( Scene::Get() )
{

}

void PlayerControllerSubSystem::Init()
{
    mKeyboard=Engine::Get().GetSystem<KeyboardSystem>();
    mMouseMoveId = EventServer<WorldMouseMoveEvent>::Get().Subscribe( boost::bind( &PlayerControllerSubSystem::OnMouseMoveEvent, this, _1 ) );

}

void PlayerControllerSubSystem::Update(Actor& actor, double DeltaTime)
{
    Opt<PlayerControllerComponent> playerControllerC = actor.Get<PlayerControllerComponent>();
    if (!playerControllerC.IsValid()||!playerControllerC->IsEnabled())
    {
        return;
    }
    SetSpeedAndOrientation(actor);
    Shoot(actor);
    SetOrientation(actor);
}

void PlayerControllerSubSystem::OnMouseMoveEvent(const WorldMouseMoveEvent& Event)
{
    mX = Event.Pos.x;
    mY = Event.Pos.y;
}

void PlayerControllerSubSystem::SetSpeedAndOrientation(Actor &actor)
{
    uint32_t mCurrentMovement = 0;
    if( mKeyboard->GetKey(GLFW_KEY_W).State==KeyState::Down)
    {
        mCurrentMovement |= MF_Up;
    }
    if( mKeyboard->GetKey(GLFW_KEY_A).State==KeyState::Down )
    {
        mCurrentMovement |= MF_Left;
    }
    if( mKeyboard->GetKey(GLFW_KEY_S).State==KeyState::Down )
    {
        mCurrentMovement |= MF_Down;
    }
    if( mKeyboard->GetKey(GLFW_KEY_D).State==KeyState::Down )
    {
        mCurrentMovement |= MF_Right;
    }

    int x = ( ( mCurrentMovement & MF_Left ) ? -1 : 0 ) + ( ( mCurrentMovement & MF_Right ) ? 1 : 0 );
    int y = ( ( mCurrentMovement & MF_Up ) ? 1 : 0 ) + ( ( mCurrentMovement & MF_Down ) ? -1 : 0 );
    actor.Get<IMoveComponent>()->SetSpeed( std::max<double>( std::abs( x ), std::abs( y ) )*.35 );

    double Heading = 0;
    static const double pi = boost::math::constants::pi<double>();
    if( x == 0 )
    {
        Heading = ( y < 0 ) ? -pi / 2 : pi / 2;
    }
    else if( y == 0 )
    {
        Heading = ( x < 0 ) ? -pi : 0;
    }
    else if( y > 0 )
    {
        Heading = ( x < 0 ) ? pi * 0.75 : pi / 4;
    }
    else
    {
        Heading = ( x < 0 ) ? pi * 1.25 : pi * 1.75;
    }
    actor.Get<IMoveComponent>()->SetHeading( Heading );
    if( x == 0 && y == 0 )
    {
        actor.AddAction( AutoId( "idle" ) );
    }
    else
    {
        actor.AddAction( AutoId( "move" ) );
    }
}

void PlayerControllerSubSystem::Shoot(Actor &actor)
{
    if (mMouse.IsButtonPressed( Mouse::Button_Left ))
    {
        actor.DropAction( AutoId( "shoot_alt" ) );
        actor.AddAction( AutoId( "shoot" ) );
    }
    else if (mMouse.IsButtonPressed( Mouse::Button_Right ))
    {
        actor.DropAction( AutoId( "shoot" ) );
        actor.AddAction( AutoId( "shoot_alt" ) );
    }
    else
    {
        actor.DropAction( AutoId( "shoot" ) );
        actor.DropAction( AutoId( "shoot_alt" ) );
    }
}

void PlayerControllerSubSystem::SetOrientation(Actor &actor)
{
    Opt<IPositionComponent> actorPositionC = actor.Get<IPositionComponent>();
    double Rot = atan2( mY - actorPositionC->GetY(), mX - actorPositionC->GetX() );
    actorPositionC->SetOrientation( Rot );
}

} // namespace engine

