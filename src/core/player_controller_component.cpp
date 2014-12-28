#include "platform/i_platform.h"
#include "input/i_input.h"
#include "core/i_position_component.h"
#include "core/i_move_component.h"
#include "core/player_controller_component.h"
#include "core/i_health_component.h"
#include "core/scene.h"

PlayerControllerComponent::PlayerControllerComponent()
    : ControllerComponent()
    , mCurrentMovement( 0 )
    , mX( 0.0 )
    , mY( 0.0 )
    , mDirty( true )
    , mMouse( Mouse::Get() )
    , mShoot( false )
    , mShootAlt( false )
{
    mKeyId = EventServer<KeyEvent>::Get().Subscribe( boost::bind( &PlayerControllerComponent::OnKeyEvent, this, _1 ) );
    mMouseMoveId = EventServer<WorldMouseMoveEvent>::Get().Subscribe( boost::bind( &PlayerControllerComponent::OnMouseMoveEvent, this, _1 ) );
    mMousePressId = EventServer<WorldMousePressEvent>::Get().Subscribe( boost::bind( &PlayerControllerComponent::OnMousePressEvent, this, _1 ) );
    mMouseReleaseId = EventServer<WorldMouseReleaseEvent>::Get().Subscribe( boost::bind( &PlayerControllerComponent::OnMouseReleaseEvent, this, _1 ) );
}

void PlayerControllerComponent::OnKeyEvent( const KeyEvent& Event )
{
    uint32_t OldMovement = mCurrentMovement;
    uint32_t Mod = 0;
    if( Event.Key == GLFW_KEY_W )
    {
        Mod = MF_Up;
    }
    else if( Event.Key == GLFW_KEY_A )
    {
        Mod = MF_Left;
    }
    else if( Event.Key == GLFW_KEY_S )
    {
        Mod = MF_Down;
    }
    else if( Event.Key == GLFW_KEY_D )
    {
        Mod = MF_Right;
    }
    if( Event.State == KeyState::Down )
    {
        mCurrentMovement |= Mod;
    }
    else
    {
        mCurrentMovement &= ~Mod;
    }
    mDirty = mDirty || mCurrentMovement != OldMovement;
}

void PlayerControllerComponent::Update( double Seconds )
{
    if( !mActor )
    {
        return;
    }
    UpdateRotation();
    if( !mDirty )
    {
        return;
    }
    
    mDirty = false;
    int x = ( ( mCurrentMovement & MF_Left ) ? -1 : 0 ) + ( ( mCurrentMovement & MF_Right ) ? 1 : 0 );
    int y = ( ( mCurrentMovement & MF_Up ) ? 1 : 0 ) + ( ( mCurrentMovement & MF_Down ) ? -1 : 0 );
    mActor->Get<IMoveComponent>()->SetSpeed( std::max<double>( std::abs( x ), std::abs( y ) )*.35 );

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

    mActor->Get<IMoveComponent>()->SetHeading( Heading );
    if( x == 0 && y == 0 )
    {
        mActor->AddAction( AutoId( "idle" ) );
    }
    else
    {
        mActor->AddAction( AutoId( "move" ) );
    }
    if ( mShoot )
    {
         mActor->AddAction( AutoId( "shoot" ) );
    }
    if ( mShootAlt )
    {
         mActor->AddAction( AutoId( "shoot_alt" ) );
    }
}
//TODO: these should only set states, all real actions should happen in Update
void PlayerControllerComponent::OnMouseMoveEvent( const WorldMouseMoveEvent& Event )
{
    mX = Event.Pos.x;
    mY = Event.Pos.y;
}

void PlayerControllerComponent::UpdateRotation()
{
    Opt<IPositionComponent> actorPositionC = mActor->Get<IPositionComponent>();
    double Rot = atan2( mY - actorPositionC->GetY(), mX - actorPositionC->GetX() );
    actorPositionC->SetOrientation( Rot );
}

void PlayerControllerComponent::OnMousePressEvent( const WorldMousePressEvent& Event )
{
    bool oldShoot=mShoot;
    bool oldShootAlt=mShootAlt;
    if ( Event.Button == Mouse::Button_Left && !mMouse.IsButtonPressed( Mouse::Button_Left ) )
    {
        mShoot = true;
    }
    else if ( Event.Button == Mouse::Button_Right && !mMouse.IsButtonPressed( Mouse::Button_Right ) )
    {
        mShootAlt = true;
    }
    mDirty=mDirty || (oldShoot!=mShoot) || (oldShootAlt!=mShootAlt);

    // ez itt pusztan funkcionalitas tesztelesre van, dummy implementacio
    static const double Cooldown = 0.0002;
    static double PrevTime = 0;
    const double CurTime = glfwGetTime();
    if( CurTime - PrevTime < Cooldown )
    {
        return;
    }
    PrevTime = CurTime;
    if ( Event.Button == Mouse::Button_Middle )
    {
        Scene::Get().AddTestCreep(mActor,Event.Pos.x,Event.Pos.y);
    }
}


void PlayerControllerComponent::OnMouseReleaseEvent( const WorldMouseReleaseEvent& Event )
{
    bool oldShoot=mShoot;
    bool oldShootAlt=mShootAlt;
    if ( Event.Button == Mouse::Button_Left )
    {
        mActor->DropAction( AutoId( "shoot" ) );
        mShoot = false;
        if ( mMouse.IsButtonPressed( Mouse::Button_Right ) )
        {
            mShootAlt = true;
         }
    }
    else if ( Event.Button == Mouse::Button_Right )
    {
        mActor->DropAction( AutoId( "shoot_alt" ) );
        mShootAlt = false;
        if ( mMouse.IsButtonPressed( Mouse::Button_Left ) )
        {
            mShoot = true;
        }
    }
    mDirty=mDirty || (oldShoot!=mShoot) || (oldShootAlt!=mShootAlt);

}

void PlayerControllerComponentLoader::BindValues()
{

}

PlayerControllerComponentLoader::PlayerControllerComponentLoader()
{

}
