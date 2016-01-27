#ifndef INCLUDED_CORE_EXPLODE_ON_DEATH_COMPONENT_H
#define INCLUDED_CORE_EXPLODE_ON_DEATH_COMPONENT_H

#include "i_explode_on_death_component.h"
#include "core/property_loader.h"

class ExplodeOnDeathComponent : public IExplodeOnDeathComponent
{
public:
    ExplodeOnDeathComponent();
};

class ExplodeOnDeathComponentLoader : public ComponentLoader<ExplodeOnDeathComponent>
{
    virtual void BindValues();
protected:
    ExplodeOnDeathComponentLoader();
    friend class ComponentLoaderFactory;
    ExplodeDistributionType& mExplodeDistributionType;
};

#endif//INCLUDED_CORE_EXPLODE_ON_DEATH_COMPONENT_H

//command:  "classgenerator.exe" -g "component" -c "explode_on_death_component" -m "int32_t-explosionProjectile int32_t-count double-scatter"
