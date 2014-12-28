#ifndef INCLUDED_INPUT_KEYBOARD_H
#define INCLUDED_INPUT_KEYBOARD_H

#include "engine/system.h"

struct KeyState
{
    enum Type
    {
        Down,
        Up,
    };
};

struct KeyEvent : public Event
{
    const int Key;
    const int Mods;
    const KeyState::Type State;
    KeyEvent( int K, int M, KeyState::Type S ): Key( K ), Mods( M ), State( S ) {}
};

struct UniCharEvent : public Event
{
    const int Codepoint;
    UniCharEvent( int Cp ) : Codepoint( Cp ) {}
};

struct Key
{
    const int Mods;
    const KeyState::Type State;
    Key( int M, KeyState::Type S ): Mods( M ), State( S ) {}
};

namespace engine {

class KeyboardSystem : public System
{
public:
    Key GetKey( int key ) const;
    void SetWindow( GLFWwindow* wnd );
    KeyboardSystem();
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    typedef std::map<int, Key> Keys_t;
    Keys_t mKeys;
    AutoReg mKeyId;
    static void KeyCallback( GLFWwindow*, int Key, int Scan, int Action, int Mods );
    static void UniCharCallback( GLFWwindow*, unsigned int Codepoint );

    void OnKeyEvent( const KeyEvent& Event );
};

} // namespace engine
#endif//INCLUDED_INPUT_KEYBOARD_H
