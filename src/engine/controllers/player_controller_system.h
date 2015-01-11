#ifndef INCLUDED_PLAYER_CONTROLLER_SYSTEM_H
#define INCLUDED_PLAYER_CONTROLLER_SYSTEM_H

#include "core/scene.h"
#include "engine/sub_system.h"
#include "input/i_input.h"
#include "core/opt.h"
#include "input/keyboard.h"

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
    void SetSpeedAndOrientation(Actor &actor);
    void SetOrientation(Actor &actor);
    void Shoot(Actor &actor);

    AutoReg mMouseMoveId;
    Mouse& mMouse;
    double mX;
    double mY;

    Scene& mScene;
    Opt<KeyboardSystem> mKeyboard;
    enum MoveFlags
    {
        MF_Up       = 1 << 0,
        MF_Down     = 1 << 1,
        MF_Left     = 1 << 2,
        MF_Right    = 1 << 3,
    };
};

} // namespace engine

#endif//INCLUDED_PLAYER_CONTROLLER_SYSTEM_H
