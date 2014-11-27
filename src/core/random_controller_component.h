#ifndef INCLUDED_CORE_RANDOM_CONTROLLER_H
#define INCLUDED_CORE_RANDOM_CONTROLLER_H

#include "core/i_controller_component.h"
#include "core/actor.h"

class RandomControllerComponent : public IControllerComponent
{
public:
    RandomControllerComponent();
    virtual void Update( double Seconds );
    virtual void SetActor( Actor* Obj );
private:
    double mCounter;
    double mHeadingModifier;
};

#endif//INCLUDED_CORE_RANDOM_CONTROLLER_H
