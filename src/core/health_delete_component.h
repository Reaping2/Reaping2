#ifndef INCLUDED_CORE_HEALTH_DELETE_COMPONENT_H
#define INCLUDED_CORE_HEALTH_DELETE_COMPONENT_H

#include "health_component.h"

class HealthDeleteComponent : public HealthComponent
{
public:
    virtual void Update( double Seconds );
protected:
    HealthDeleteComponent();
    friend class ComponentFactory;
};

#endif//INCLUDED_CORE_HEALTH_DELETE_COMPONENT_H