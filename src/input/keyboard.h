#ifndef INCLUDED_INPUT_KEYBOARD_H
#define INCLUDED_INPUT_KEYBOARD_H


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

class Keyboard : public Singleton<Keyboard>
{
public:
    void SetWindow( GLFWwindow* wnd );
private:
    friend class Singleton<Keyboard>;
    Keyboard();

    static void KeyCallback( GLFWwindow*, int Key, int Scan, int Action, int Mods );
    static void UniCharCallback( GLFWwindow*, unsigned int Codepoint );
};

#endif//INCLUDED_INPUT_KEYBOARD_H
