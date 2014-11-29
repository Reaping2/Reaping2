#ifndef INCLUDED_CORE_CONTROLLER_COMPONENT_H
#define INCLUDED_CORE_CONTROLLER_COMPONENT_H

#include "core/i_controller_component.h"
#include "core/actor.h"

class ControllerComponent : public IControllerComponent
{
public:
    ControllerComponent();
    virtual void SetEnabled( bool Enabled );
    virtual bool IsEnabled();
protected:
    bool mEnabled;
};

#endif//INCLUDED_CORE_CONTROLLER_COMPONENT_H
