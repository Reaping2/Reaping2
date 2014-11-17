#ifndef INCLUDED_CORE_TARGET_PLAYER_CONTROLLER_H
#define INCLUDED_CORE_TARGET_PLAYER_CONTROLLER_H

#include "core/i_controller_component.h"
#include "core/actor.h"

class TargetPlayerControllerComponent : public IControllerComponent
{
public:
    TargetPlayerControllerComponent();
    virtual void Update( double Seconds );
    virtual void SetActor( Actor* Obj );
    void SetPlayer( Actor* Player );
private:
    Actor* mActor;
    Actor* mPlayer;
    double mCounter;
    double mHeadingModifier;
};

#endif//INCLUDED_CORE_TARGET_PLAYER_CONTROLLER_H
