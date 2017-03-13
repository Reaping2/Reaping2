#include "i_input.h"
#include "main/window.h"
#include "core/opt.h"
#include "engine/engine.h"
namespace engine {
MouseSystem::MouseSystem()
    : mHeldButtons( 0 )
    , mMousePressEventWait( 0 )
    , mMousePressEventRepeatInterval( 0.1 )
{
}
void MouseSystem::Init()
{
    Opt<WindowSystem> Wind( Engine::Get().GetSystem<WindowSystem>() );
    GLFWwindow* Wnd = Wind->GetWindow();
    glfwSetCursorPosCallback( Wnd, &MouseSystem::OnMouseMove );
    glfwSetMouseButtonCallback( Wnd, &MouseSystem::OnMouseButton );
    glfwSetCursorEnterCallback( Wnd, &MouseSystem::OnMouseEnter );
    glfwSetScrollCallback( Wnd, &MouseSystem::OnMouseScroll );
}


void MouseSystem::OnMouseMove( GLFWwindow* Wnd, double x, double y )
{
    Opt<MouseSystem> M( engine::Engine::Get().GetSystem<MouseSystem>() );
    M->mRawMouseCoord.x = ( float )x;
    M->mRawMouseCoord.y = ( float )y;
    EventServer<ScreenMouseMoveEvent>::Get().SendEvent( ScreenMouseMoveEvent( M->mRawMouseCoord ) );
}

void MouseSystem::OnMouseButton( GLFWwindow*, int button, int action, int mods )
{
    engine::Engine::Get().GetSystem<MouseSystem>()->MousePressed( button, action, mods );
}

void MouseSystem::MousePressed( int button, int action, int /*mods*/ )
{
    Button_t Button = Button_Left;
    if (Button_Left <= button&&button < Num_Buttons)
    {
        Button = Button_t( button );
    }
    if( action == GLFW_PRESS )
    {
        mRawPressCoord = mRawMouseCoord;
        mMousePressEventWait = mMousePressEventRepeatInterval;
        EventServer<ScreenMousePressEvent>::Get().SendEvent( ScreenMousePressEvent( mRawMouseCoord, Button ) );
        mHeldButtons |= 1 << Button;
    }
    else if( action == GLFW_RELEASE )
    {
        EventServer<ScreenMouseReleaseEvent>::Get().SendEvent( ScreenMouseReleaseEvent( mRawMouseCoord, Button ) );
        const glm::vec2 Diff = mRawMouseCoord - mRawPressCoord;
        const float DistSqr = glm::dot( Diff, Diff );
        static const float MinDistForDrag = 25.f;
        glm::vec2 Out;
        if( DistSqr >= MinDistForDrag )
        {
            EventServer<ScreenMouseDragEvent>::Get().SendEvent( ScreenMouseDragEvent( mRawPressCoord, mRawMouseCoord, Button ) );
        }
        mHeldButtons &= ~( 1 << Button );
    }
    //    LOG( "Held buttons:%d", mHeldButtons );

}

bool MouseSystem::IsButtonPressed( Button_t Button ) const
{
    return !( !( mHeldButtons & ( 1 << Button ) ) );
}

void MouseSystem::Update( double DeltaTime )
{
    if( mMousePressEventWait > DeltaTime )
    {
        mMousePressEventWait -= DeltaTime;
        return;
    }
    for( size_t i = 1; i < Num_Buttons; ++i )
    {
        const Button_t Button( ( Button_t )i );
        if( IsButtonPressed( Button ) )
        {
            //LOG( "Held buttons and is pressed:%d,%d,%d \n", mHeldButtons, !( !mHeldButtons & ( 1 << Button ) ), Button );
            EventServer<ScreenMousePressEvent>::Get().SendEvent( ScreenMousePressEvent( mRawMouseCoord, Button ) );
        }
    }
    mMousePressEventWait = mMousePressEventRepeatInterval;
}

void MouseSystem::OnMouseEnter( GLFWwindow*, int entered )
{
    EventServer<MouseEnterEvent>::Get().SendEvent( MouseEnterEvent( entered == GL_TRUE ) );
}

void MouseSystem::OnMouseScroll( GLFWwindow*, double x_offs, double y_offs )
{
    // itt nincs space transition, az offsetek a gorgotol fuggnek csak
    EventServer<MouseScrollEvent>::Get().SendEvent( MouseScrollEvent( glm::vec2( x_offs, y_offs ) ) );
}


} // namespace engine
MouseMoveEvent::~MouseMoveEvent()
{

}

MousePressEvent::~MousePressEvent()
{

}

MouseReleaseEvent::~MouseReleaseEvent()
{

}

MouseDragEvent::~MouseDragEvent()
{

}
