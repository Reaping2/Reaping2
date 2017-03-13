#ifndef INCLUDED_ENGINE_KEYBOARD_AND_MOUSE_ADAPTER_SYSTEM_H
#define INCLUDED_ENGINE_KEYBOARD_AND_MOUSE_ADAPTER_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"
#include "keyboard.h"
#include "input_system.h"
#include "mouse.h"
#include <map>

namespace engine {

    struct DesktopInputState
    {
        Json::Value const& scheme;
        Opt<KeyboardSystem> mKeyboard;
        Opt<MouseSystem> mMouse;
        std::map<std::string, int32_t> mKeyMap =
        {
            { "space",GLFW_KEY_SPACE },
            { "tab",GLFW_KEY_TAB },
            { "escape",GLFW_KEY_ESCAPE },
        };
        DesktopInputState();
        void Init();
        bool isValid();
        bool operator()( std::string const& valname );
    };

class KeyboardAndMouseAdapterSystem : public System
{
public:
    DEFINE_SYSTEM_BASE(KeyboardAndMouseAdapterSystem)
    KeyboardAndMouseAdapterSystem();
    enum MoveFlags
    {
        MF_Up = 1 << 0,
        MF_Down = 1 << 1,
        MF_Left = 1 << 2,
        MF_Right = 1 << 3,
    };
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Opt<KeyboardSystem> mKeyboard;
    Opt<MouseSystem> mMouse;
    Opt<InputSystem> mInputSystem;
    Scene& mScene = Scene::Get();
    core::ProgramState& mProgramState = core::ProgramState::Get();
    double mX = 0.0;
    double mY = 0.0;
    AutoReg mMouseMoveId;
    DesktopInputState mDesktopState;
    int32_t mPlayerId = 1; // TODO: temp
    void OnMouseMoveEvent( const WorldMouseMoveEvent& Event );
    void HandleKeyboard();
    void HandleMouse();
};

} // namespace engine

#endif//INCLUDED_ENGINE_KEYBOARD_AND_MOUSE_ADAPTER_SYSTEM_H

//command:  "classgenerator.exe" -g "system" -n "engine" -c "keyboard_and_mouse_adapter_system"
