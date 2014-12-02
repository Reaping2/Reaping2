#ifndef INCLUDED_CORE_RANDOM_CONTROLLER_H
#define INCLUDED_CORE_RANDOM_CONTROLLER_H

#include "core/controller_component.h"
#include "core/actor.h"
#include "core/property_loader.h"

class RandomControllerComponent : public ControllerComponent
{
public:
    RandomControllerComponent();
    virtual void Update( double Seconds );
    virtual void SetActor( Actor* Obj );
private:
    double mCounter;
    double mHeadingModifier;
};

class RandomControllerComponentLoader: public ComponentLoader<RandomControllerComponent>
{
    virtual void BindValues();
protected:
    RandomControllerComponentLoader();
    friend class ComponentLoaderFactory;
};

#endif//INCLUDED_CORE_RANDOM_CONTROLLER_H
