#ifndef INCLUDED_CORE_PLAYER_CONTROLLER_H
#define INCLUDED_CORE_PLAYER_CONTROLLER_H

#include "input/i_input.h"
#include "core/controller_component.h"
#include "core/actor.h"

class PlayerControllerComponent : public ControllerComponent
{
public:
    PlayerControllerComponent();
    virtual void Update( double Seconds );
private:
    void OnKeyEvent( const KeyEvent& Event );
    void OnMouseMoveEvent( const WorldMouseMoveEvent& Event );
    void OnMousePressEvent( const WorldMousePressEvent& Event );
    void OnMouseReleaseEvent( const WorldMouseReleaseEvent& Event );
    void UpdateRotation();
    enum MoveFlags
    {
        MF_Up       = 1 << 0,
        MF_Down     = 1 << 1,
        MF_Left     = 1 << 2,
        MF_Right    = 1 << 3,
    };
    uint32_t mCurrentMovement;
    double mX;
    double mY;
    bool mDirty;
    Mouse& mMouse;
    AutoReg mKeyId;
    AutoReg mMouseMoveId;
    AutoReg mMousePressId;
    AutoReg mMouseReleaseId;

};

#endif//INCLUDED_CORE_PLAYER_CONTROLLER_H
