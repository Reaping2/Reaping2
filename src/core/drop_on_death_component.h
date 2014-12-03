#ifndef INCLUDED_CORE_DROP_ON_DEATH_COMPONENT_H
#define INCLUDED_CORE_DROP_ON_DEATH_COMPONENT_H

#include "core/i_drop_on_death_component.h"
#include "core/property_loader.h"

class DropOnDeathComponent : public IDropOnDeathComponent
{
public:
    virtual void Update( double Seconds );

    DropOnDeathComponent();
    friend class ComponentFactory;

    bool mTriedDrop;
};

class DropOnDeathComponentLoader: public ComponentLoader<DropOnDeathComponent>
{
    virtual void BindValues();
public:
    DropOnDeathComponentLoader();
    friend class ComponentLoaderFactory;
};

#endif//INCLUDED_CORE_DROP_ON_DEATH_COMPONENT_H