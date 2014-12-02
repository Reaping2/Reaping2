#ifndef INCLUDED_CORE_HEALTH_DELETE_COMPONENT_H
#define INCLUDED_CORE_HEALTH_DELETE_COMPONENT_H

#include "health_component.h"
#include "property_loader.h"

class HealthDeleteComponent : public HealthComponent
{
public:
    virtual void Update( double Seconds );
protected:
    HealthDeleteComponent();
    friend class ComponentFactory;
};

class HealthDeleteComponentLoader: public ComponentLoader<HealthDeleteComponent>
{
    virtual void BindValues();
protected:
    HealthDeleteComponentLoader();
    friend class ComponentLoaderFactory;
};
#endif//INCLUDED_CORE_HEALTH_DELETE_COMPONENT_H