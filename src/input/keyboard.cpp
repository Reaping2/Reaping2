#include "i_input.h"
#include "main\window.h"
#include <boost/bind.hpp>
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
    SetWindow(Window::Get().GetWindow());
}

void KeyboardSystem::Update(double DeltaTime)
{

}

void KeyboardSystem::OnKeyEvent(const KeyEvent& Event)
{
    Keys_t::iterator it = mKeys.find(Event.Key);
    if (Event.State==KeyState::Down)
    {
        if (it==mKeys.end())
        {
            mKeys.insert(Keys_t::value_type(Event.Key,Key(Event.Mods,Event.State)));
        }
    }
    else
    {
        mKeys.erase(it);
    }
}

Key KeyboardSystem::GetKey(int key) const
{
    Keys_t::const_iterator it = mKeys.find(key);
    return it!=mKeys.end()?it->second:Key(0,KeyState::Up);
}

} // namespace engine
