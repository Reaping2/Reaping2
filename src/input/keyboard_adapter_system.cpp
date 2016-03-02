#include "platform/i_platform.h"
#include "keyboard_adapter_system.h"
#include "engine/engine.h"
#include "core/i_position_component.h"

namespace engine {

KeyboardAdapterSystem::KeyboardAdapterSystem()
    : mScene( Scene::Get() )
    , mProgramState( core::ProgramState::Get() )
{
}


void KeyboardAdapterSystem::Init()
{
    mKeyboard=Engine::Get().GetSystem<KeyboardSystem>();
    mInputSystem=InputSystem::Get();
}


void KeyboardAdapterSystem::Update(double DeltaTime)
{
    uint32_t currentMovement = 0;
    if( mKeyboard->GetKey(GLFW_KEY_W).State==KeyState::Down)
    {
        currentMovement |= MF_Up;
    }
    if( mKeyboard->GetKey(GLFW_KEY_A).State==KeyState::Down )
    {
        currentMovement |= MF_Left;
    }
    if( mKeyboard->GetKey(GLFW_KEY_S).State==KeyState::Down )
    {
        currentMovement |= MF_Down;
    }
    if( mKeyboard->GetKey(GLFW_KEY_D).State==KeyState::Down )
    {
        currentMovement |= MF_Right;
    }

    InputState inputState=mInputSystem->GetInputState();
    if( mKeyboard->GetKey(GLFW_KEY_Q).State==KeyState::Typed /*obsolete enabled for one iteration*/
        ||mKeyboard->GetKey(GLFW_KEY_SPACE).State==KeyState::Down)
    {
        inputState.mUseNormalItem=true;
    }
    if( mKeyboard->GetKey(GLFW_KEY_R).State==KeyState::Down )
    {
        inputState.mReload=true;
    }

    int x = ( ( currentMovement & MF_Left ) ? -1 : 0 ) + ( ( currentMovement & MF_Right ) ? 1 : 0 );
    int y = ( ( currentMovement & MF_Up ) ? 1 : 0 ) + ( ( currentMovement & MF_Down ) ? -1 : 0 );

    inputState.mMoving=(std::max<double>( std::abs( x ), std::abs( y ) )!=0);

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
    inputState.mHeading=Heading;

    if( mKeyboard->GetKey(GLFW_KEY_TAB).State==KeyState::Down)
    {
         inputState.mShowLeaderboard=true;
    }
    if( mKeyboard->GetKey(GLFW_KEY_ESCAPE).State==KeyState::Typed)
    {
         inputState.mPause=true;
    }
    mInputSystem->SetInputState(inputState);
}


} // namespace engine

