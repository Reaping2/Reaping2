#include "platform/i_platform.h"
#include "keyboard_and_mouse_adapter_system.h"
#include "engine/engine.h"
#include "mapping.h"
#include "mouse.h"
#include "player_control_device.h"
#include "core/i_position_component.h"

namespace engine {

KeyboardAndMouseAdapterSystem::KeyboardAndMouseAdapterSystem()
{

}


void KeyboardAndMouseAdapterSystem::Init()
{
    mKeyboard = Engine::Get().GetSystem<KeyboardSystem>();
    mMouse = Engine::Get().GetSystem<MouseSystem>();
    mInputSystem = InputSystem::Get();
    mMouseMoveId = EventServer<WorldMouseMoveEvent>::Get().Subscribe( boost::bind( &KeyboardAndMouseAdapterSystem::OnMouseMoveEvent, this, _1 ) );
    mDesktopState.Init();
}


void KeyboardAndMouseAdapterSystem::Update(double DeltaTime)
{
    static input::PlayerControlDevice& pcd( input::PlayerControlDevice::Get() );
    if (pcd.GetControlDevice( mPlayerId ) != input::PlayerControlDevice::KeyboardAndMouse)
    {
        return;
    }
    HandleKeyboard();
    HandleMouse();
}


void KeyboardAndMouseAdapterSystem::OnMouseMoveEvent( const WorldMouseMoveEvent& Event )
{
    mX = Event.Pos.x;
    mY = Event.Pos.y;
}

void KeyboardAndMouseAdapterSystem::HandleKeyboard()
{
    InputState inputState = mInputSystem->GetInputState( mPlayerId );
    inputState.mUseNormalItem = mDesktopState( "use_normal" );
    inputState.mActivate = mDesktopState( "activate" );
    inputState.mReload = mDesktopState( "reload" );
    inputState.mSwitchWeapon = mDesktopState( "switch_weapon" );
    inputState.mSwitchNormalItem = mDesktopState( "switch_item" );
    inputState.mShoot = mDesktopState( "shoot" );
    inputState.mShootAlt = mDesktopState( "shoot_alt" );
    inputState.mShowLeaderboard = mDesktopState( "show_leaderboard" );
    inputState.mPause = mDesktopState( "pause" );

    uint32_t currentMovement = 0;
    if (mDesktopState( "move_up" ))
    {
        currentMovement |= MF_Up;
    }
    if (mDesktopState( "move_left" ))
    {
        currentMovement |= MF_Left;
    }
    if (mDesktopState( "move_down" ))
    {
        currentMovement |= MF_Down;
    }
    if (mDesktopState( "move_right" ))
    {
        currentMovement |= MF_Right;
    }
    int x = ((currentMovement & MF_Left) ? -1 : 0) + ((currentMovement & MF_Right) ? 1 : 0);
    int y = ((currentMovement & MF_Up) ? 1 : 0) + ((currentMovement & MF_Down) ? -1 : 0);

    inputState.mMoving = (std::max<double>( std::abs( x ), std::abs( y ) ) != 0);

    double Heading = 0;
    static const double pi = boost::math::constants::pi<double>();
    if (x == 0)
    {
        Heading = (y < 0) ? -pi / 2 : pi / 2;
    }
    else if (y == 0)
    {
        Heading = (x < 0) ? -pi : 0;
    }
    else if (y > 0)
    {
        Heading = (x < 0) ? pi * 0.75 : pi / 4;
    }
    else
    {
        Heading = (x < 0) ? pi * 1.25 : pi * 1.75;
    }
    inputState.mHeading = Heading;

    Opt<Actor> actor( mScene.GetActor( mProgramState.mControlledActorGUID ) );
    if (actor.IsValid())
    {
        Opt<IPositionComponent> actorPositionC = actor->Get<IPositionComponent>();
        inputState.mOrientation = atan2( mY - actorPositionC->GetY(), mX - actorPositionC->GetX() );
    }
    mInputSystem->SetInputState( mPlayerId, inputState );
}

void KeyboardAndMouseAdapterSystem::HandleMouse()
{
    InputState inputState = mInputSystem->GetInputState( mPlayerId );

    inputState.mCursorX = mX;
    inputState.mCursorY = mY;

    mInputSystem->SetInputState( mPlayerId, inputState );
}

DesktopInputState::DesktopInputState() 
    : scheme( input::Mapping::Get().getMapping( "keyboard_and_mouse" ) )
{

}

void DesktopInputState::Init()
{
    mKeyboard = Engine::Get().GetSystem<KeyboardSystem>();
    mMouse = Engine::Get().GetSystem<MouseSystem>();
}

bool DesktopInputState::isValid()
{
    return !!scheme;
}

bool DesktopInputState::operator()( std::string const& valname )
{
    if (!isValid())
    {
        L1( "not valid\n" );
        return false;
    }
    Json::Value val = scheme[valname];
    std::string str = val.asString();
    try
    {
        char const c = str[0];
        if (str.length() == 1&&'a'<=c&&c<='z')
        {
            char const chr = (str[0] - 'a' + GLFW_KEY_A);
            return mKeyboard->GetKey( chr ).State == KeyState::Down;
        }
        else if (str.length() == 3 && boost::starts_with( str, "mb" ))
        {
            return mMouse->IsButtonPressed(
                MouseSystem::Button_t( std::stoi( str.substr( 2 ) ) - 1 ) );
        }
        else
        {
            auto const found = mKeyMap.find( str );
            if (found != mKeyMap.end())
            {
                return mKeyboard->GetKey( found->second ).State == KeyState::Down;
            }
        }
    }
    catch (...) {}
    return false;
}

} // namespace engine

