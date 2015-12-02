#ifndef INCLUDED_ENGINE_CONTROLLERS_PLAYER_CONTROLLER_SUB_SYSTEM_H
#define INCLUDED_ENGINE_CONTROLLERS_PLAYER_CONTROLLER_SUB_SYSTEM_H

#include "core/scene.h"
#include "engine/sub_system.h"
#include "input/i_input.h"
#include "core/opt.h"
#include "input/keyboard.h"
#include "core/player_controller_component.h"
#include "core/program_state.h"

enum MoveFlags
{
    MF_Up       = 1 << 0,
    MF_Down     = 1 << 1,
    MF_Left     = 1 << 2,
    MF_Right    = 1 << 3,
};

namespace engine {

class PlayerControllerSubSystem : public SubSystem
{
public:
    DEFINE_SUB_SYSTEM_BASE(PlayerControllerSubSystem)
    PlayerControllerSubSystem();
    virtual void Init();
    virtual void Update( Actor& actor, double DeltaTime );

private:
    void OnMouseMoveEvent( const WorldMouseMoveEvent& Event );
    void SetSpeedAndOrientation(Actor &actor, Opt<PlayerControllerComponent> playerControllerC);
    void SetOrientation(Actor &actor, Opt<PlayerControllerComponent> playerControllerC);
    void Shoot(Actor &actor, Opt<PlayerControllerComponent> playerControllerC);
    void HandleRevive(Actor &actor, Opt<PlayerControllerComponent> playerControllerC);
    void HandleInputs(Actor &actor, Opt<PlayerControllerComponent> playerControllerC);
    void HandleReload(Actor& actor, Opt<PlayerControllerComponent> playerControllerC);
    AutoReg mMouseMoveId;
    Opt<MouseSystem> mMouse;
    double mX;
    double mY;

    Scene& mScene;
    core::ProgramState& mProgramState;
    Opt<KeyboardSystem> mKeyboard;

    AutoReg mKeyId;
    bool mSpaceTyped;
    bool mReloadTyped;
    void OnKeyEvent( const KeyEvent& Event );

};

} // namespace engine

#endif//INCLUDED_ENGINE_CONTROLLERS_PLAYER_CONTROLLER_SUB_SYSTEM_H
