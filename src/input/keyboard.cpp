#include "i_input.h"
#include "main/window.h"
#include <boost/bind.hpp>
#include "engine/engine.h"
namespace engine {

void KeyboardSystem::KeyCallback( GLFWwindow*, int Key, int Scan, int Action, int Mods )
{
    if( Action == GLFW_REPEAT )
    {
        return;    // no need
    }
    if( Key == GLFW_KEY_UNKNOWN )
    {
        return;
    }
    EventServer<KeyEvent>::Get().SendEvent( KeyEvent( Key, Mods, Action == GLFW_PRESS ? KeyState::Down : KeyState::Up ) );
}

void KeyboardSystem::UniCharCallback( GLFWwindow*, unsigned int Codepoint )
{
    EventServer<UniCharEvent>::Get().SendEvent( UniCharEvent( Codepoint ) );
}

KeyboardSystem::KeyboardSystem()
{
}

void KeyboardSystem::SetWindow( GLFWwindow* Wnd )
{
    glfwSetKeyCallback( Wnd, &KeyboardSystem::KeyCallback );
    glfwSetCharCallback( Wnd, &KeyboardSystem::UniCharCallback );
}

void KeyboardSystem::Init()
{
    mKeyId = EventServer<KeyEvent>::Get().Subscribe( boost::bind( &KeyboardSystem::OnKeyEvent, this, _1 ) );
    SetWindow( Engine::Get().GetSystem<WindowSystem>()->GetWindow() );
}

void KeyboardSystem::Update( double DeltaTime )
{
    for( Keys_t::iterator i = mKeys.begin(), e = mKeys.end(), n; ( i != e ? ( n = i, ++n, true ) : false ); i = n )
    {
        if( i->second.State == KeyState::Typed )
        {
            mKeys.erase( i );
        }
    }
    for ( Keys_t::iterator i = mReleasedKeys.begin(), e = mReleasedKeys.end(); i != e; ++i )
    {
        mKeys.insert( *i );
    }
    mReleasedKeys.clear();
}

void KeyboardSystem::OnKeyEvent( const KeyEvent& Event )
{
    Keys_t::iterator it = mKeys.find( Event.Key );
    if ( Event.State == KeyState::Down )
    {
        if ( it == mKeys.end() || it->second.State == KeyState::Typed )
        {
            if ( it != mKeys.end() )
            {
                mKeys.erase( it );
            }
            mKeys.insert( Keys_t::value_type( Event.Key, Key( Event.Mods, Event.State ) ) );
        }
    }
    else
    {
        if ( it != mKeys.end() )
        {
            mKeys.erase( it );
            mReleasedKeys.insert( Keys_t::value_type( Event.Key, Key( Event.Mods, KeyState::Typed ) ) );
        }
    }
}

Key KeyboardSystem::GetKey( int key ) const
{
    Keys_t::const_iterator it = mKeys.find( key );
    return it != mKeys.end() ? it->second : Key( 0, KeyState::Up );
}

} // namespace engine
