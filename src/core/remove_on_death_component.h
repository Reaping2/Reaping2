#ifndef INCLUDED_CORE_REMOVE_ON_DEATH_COMPONENT_H
#define INCLUDED_CORE_REMOVE_ON_DEATH_COMPONENT_H

#include "core/i_remove_on_death_component.h"
#include "core/property_loader.h"

class RemoveOnDeathComponent : public IRemoveOnDeathComponent
{
public:
    virtual void Update( double Seconds );
    RemoveOnDeathComponent();
    friend class ComponentFactory;
};

class RemoveOnDeathComponentLoader: public ComponentLoader<RemoveOnDeathComponent>
{
    virtual void BindValues();
public:
    RemoveOnDeathComponentLoader();
    friend class ComponentLoaderFactory;
};

#endif//INCLUDED_CORE_REMOVE_ON_DEATH_COMPONENT_H