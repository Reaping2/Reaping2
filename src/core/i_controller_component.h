#ifndef INCLUDED_CORE_I_CONTROLLER_COMPONENT_H
#define INCLUDED_CORE_I_CONTROLLER_COMPONENT_H

#include "component.h"

class Actor;
class IControllerComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IControllerComponent)
    virtual void SetActor( Actor* Obj )=0;
    virtual void Update( double Seconds )=0;
protected:
    friend class ComponentFactory;
};

#endif//INCLUDED_CORE_I_CONTROLLER_COMPONENT_H
