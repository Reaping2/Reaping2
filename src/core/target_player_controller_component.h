#ifndef INCLUDED_CORE_TARGET_PLAYER_CONTROLLER_H
#define INCLUDED_CORE_TARGET_PLAYER_CONTROLLER_H

#include "core/controller_component.h"
#include "core/actor.h"
#include "core/property_loader.h"

class TargetPlayerControllerComponent : public ControllerComponent
{
public:
    TargetPlayerControllerComponent();
    virtual void Update( double Seconds );
    virtual void SetActor( Actor* Obj );
    void SetPlayer( Actor* Player );
private:
    Actor* mPlayer;
    double mCounter;
    double mHeadingModifier;
};

class TargetPlayerControllerComponentLoader: public ComponentLoader<TargetPlayerControllerComponent>
{
    virtual void BindValues();
protected:
    TargetPlayerControllerComponentLoader();
    friend class ComponentLoaderFactory;
};

#endif//INCLUDED_CORE_TARGET_PLAYER_CONTROLLER_H
